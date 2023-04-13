#ifndef LIBSPINE_COMMON_FEC_CRC_H_
#define LIBSPINE_COMMON_FEC_CRC_H_

#include <array>
#include <ILFSR.h>

static const uint16_t kCrcPolynomial = 0x1021;

#ifdef DEVELOPMENT_CRC
static const uint16_t kCrcInit = 0x0BA5; // Development CRC
#else
static const uint16_t kCrcInit = 0xFFEA;
#endif

static const uint16_t kFecPolynomial = 0x11;
static const uint16_t kFecKeyLength = 0x9;
static const uint16_t kFecInit = 0;
static const uint16_t kFecKeyLeftAdjust = kWordLength - kFecKeyLength;
static const uint16_t kFecBitLength = 512;
static const uint16_t kFecWordLength = kFecBitLength / 16;
static const uint16_t kFecSyndromeMask = kFecBitLength - 1;

extern std::array<uint16_t, kFecBitLength> FecSyndromeLookup;

INTERFACE(ILFSREncoder) {
public:
  void ConfigureDecoder()   { return STATIC_DISPATCH(ConfigureDecoder()); }
  void ConfigureEncoder()   { return STATIC_DISPATCH(ConfigureEncoder()); }
  void Set(uint16_t value)  { return STATIC_DISPATCH(Set(value)); }
  void Reset()              { return STATIC_DISPATCH(Reset()); }
  void Push(uint16_t value) { return STATIC_DISPATCH(Push(value)); }
  uint16_t Get()            { return STATIC_DISPATCH(Get()); }
};

class NullEncoder : public ILFSREncoder<NullEncoder> {
public:
  void ConfigureDecoder() {}
  void ConfigureEncoder() {}
  void Set(uint16_t) {}
  void Reset() {}
  void Push(uint16_t) {}
  uint16_t Get() { return 0; }

  static NullEncoder& GetInstance() {
    static NullEncoder null_enc;
    return null_enc;
  }
};

template <class LFSRTy, typename mode_t, uint16_t kKeyLen, uint16_t kWordLen,
          uint16_t kPoly, uint16_t kInit>
class EncoderImpl
    : public ILFSREncoder<
          EncoderImpl<LFSRTy, mode_t, kKeyLen, kWordLen, kPoly, kInit>> {
public:
  using LFSR = ILFSR<LFSRTy>;

  constexpr EncoderImpl(LFSR& lfsr, mode_t encode_mode, mode_t decode_mode) :
    lfsr_(lfsr), encode_mode_(encode_mode), decode_mode_(decode_mode)
  {}

  void ConfigureEncoder()
  {
    lfsr_.Configure(encode_mode_, kKeyLen, kWordLen, kPoly);
    lfsr_.Set(kInit);
  }

  void ConfigureDecoder()
  {
    lfsr_.Configure(decode_mode_, kKeyLen, kWordLen, kPoly);
    lfsr_.Set(kInit);
  }

  void Set(uint16_t value)
  {
    lfsr_.Set(value);
  }

  void Reset()
  {
    lfsr_.Set(kInit);
  }

  // This will configure the decoder for a final kKeyLen bits.
  // This is useful for the FEC case where you are pushing 16-bit words
  // typically, but at the end need to push a smaller keyword.
  uint16_t FinalizeDecode(uint16_t keyword)
  {
    lfsr_.Configure(decode_mode_, kKeyLen, kKeyLen, kPoly);
    lfsr_.Push(keyword);
    return lfsr_.Get();
  }

  template <typename ...Values>
  void Push(Values... values) {
    return PushImpl(values...);
  }

  uint16_t Get()
  {
    return lfsr_.Get();
  }

private:
  template <typename ...Values>
  void PushImpl(uint16_t value, Values... values)
  {
    lfsr_.Push(value);
    Push(values...);
  }

  void PushImpl() {}

  LFSR& lfsr_;
  mode_t const encode_mode_;
  mode_t const decode_mode_;
};

template <class LFSRTy, typename mode_t>
using FEC = EncoderImpl<LFSRTy, mode_t, kFecKeyLength, kWordLength,
  kFecPolynomial, kFecInit>;

template <class L, typename mode_t>
constexpr FEC<L, mode_t> MakeFEC(L& l, mode_t enc, mode_t dec)
{
  return {l, enc, dec};
}

template <class LFSRTy, typename mode_t>
using CRC = EncoderImpl<LFSRTy, mode_t, kWordLength, kWordLength,
  kCrcPolynomial, kCrcInit>;

template <class L, typename mode_t>
constexpr CRC<L, mode_t> MakeCRC(L& l, mode_t enc)
{
  return {l, enc, enc};
}

// Attempt FEC, return true if successful
// crc = Crc16Lfsr type for use in re-testing
// crc_result = crc result to use in checking
// crc_finalizer = function to finalize crc after pushing buffer
// syndrome = FEC syndrome
// buffer = iterable buffer
// N = size of buffer we are correcting
template <class CRC, class F, class T>
bool AttemptFec(CRC& crc, uint16_t crc_result, F crc_finalizer,
                uint16_t syndrome, T& buffer) {
  syndrome &= kFecSyndromeMask;
  uint16_t lookup = FecSyndromeLookup[syndrome];
  size_t bit_index = lookup & 0xF;

  // the syndrome lookup assumes a kFecBitLength bitlength -- we need to add
  // the difference in word index as it will assume
  // we padded with kFecBitLength - N*16 zeroes.  We also need to add +1 for the CRC
  size_t word_offset = (buffer.size() - (kFecBitLength / kWordLength)) + 1;
  size_t word_index = ((lookup >> 4) & 0x1F) + word_offset;

  if (word_index > buffer.size())
    return false;

  // The CRC is protected by FEC too, so if we get N as the result, flip a bit
  // in the CRC instead
  if (word_index == buffer.size()) {
    crc_result ^= (1 << bit_index);
  } else {
    *(buffer.begin() + word_index) ^= (1 << bit_index);
  }


  crc.ConfigureDecoder();
  for (auto word : buffer) {
    crc.Push(word);
  }
  crc_finalizer();
  crc.Push(crc_result);

  return crc.Get() == 0;
}

template <class FECen>
void GenerateFec(FECen &fec) {
  // Loop over all 511 bits of the possible incoming packet
  for (size_t w = 0; w < FecSyndromeLookup.size() / 16; ++w) {
    for (size_t b = 15; b <= 15; b--) {
      // Get the current bit index
      uint16_t bit_index = (w << 4) | b;

      // Note that the 512th bit is not FEC-able, so skip it
      // The 512th bit is actually bit 496, this is because
      // the bits are loaded MSbit first into the LFSR
      if (bit_index == kFecBitLength - 16)
        break;

      // Pretend to receive a packet of 512 bits, flipping
      // the n-th bit
      fec.ConfigureDecoder();
      for (size_t j = 0; j < (FecSyndromeLookup.size() / 16); ++j) {
        if (w != j)
          fec.Push(0);
        else
          fec.Push(1 << b);
      }
      fec.FinalizeDecode(0);

      // The resulting LFSR value (fec.Get()) will be the index
      // to set inside the FEC syndrome buffer -- the value is
      // the current bit we are on
      // This correlates "what will the FEC LFSR look like when bit N is wrong?"
      size_t syndrome_index = fec.Get() & kFecSyndromeMask;
      FecSyndromeLookup[syndrome_index] = bit_index;
    }
  }
}

#endif


