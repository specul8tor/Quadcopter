#ifndef LIBSPINE_COMMON_W_OUT_STREAM_H_
#define LIBSPINE_COMMON_W_OUT_STREAM_H_

#include <type_traits>

#include "IBBU.h"
#include "SpineMath.h"
#include "FEC_CRC.h"

// EncodeIn proxy class for streaming
//   win << EncodeIn(out_var);   // writes to BBU and pumps FEC/CRC LFSRs
//   win << NoEncodeIn(out_var); // only writes to BBU
template <bool Encode>
class EncodeOutProxy {
public:
  constexpr EncodeOutProxy(uint16_t value) : value_(value) {}

  uint16_t GetValue() const { return value_; }

private:
  uint16_t value_;
};

using EncodeOut = EncodeOutProxy<true>;
using NoEncodeOut = EncodeOutProxy<false>;

template <typename Packet>
class PreambleOutImpl {
public:
  static const uint16_t kNumPreambleWords = Packet::kNumPreambleWords;
  static const uint16_t kPreamble = Packet::kPreamble;
  static const uint16_t kStartWord = Packet::kStartWord;
};

template <typename Packet>
constexpr PreambleOutImpl<Packet> PreambleOut(const Packet&) {
  return {};
}


template <typename BBUTy, typename CRC, typename FEC> class WOutStream {
public:
  using BBU = IBaseBandUnit<BBUTy>;

  constexpr WOutStream(BBU &bbu, CRC &crc, FEC &fec) :
    bbu_(bbu), crc_(crc), fec_(fec)
  {}

  ~WOutStream() {
    crc_.Push(0);
    uint16_t crc = crc_.Get();
    bbu_.PushWord(crc);
    // If the FEC was nulled out, don't transmit it
    // this supports older protocols that didnt have
    // FEC implemented
    if (!std::is_same<FEC, NullEncoder>::value) {
      fec_.Push(crc);
      // The fec keyword needs to be right aligned, so rotate into place
      // there shouldn't be any other set bits, so rotate will be fine
      bbu_.PushWord(_rol(fec_.Get(), kFecKeyLeftAdjust));
    }
  }

  void PushWord(uint16_t value) {
    bbu_.PushWord(value);
    crc_.Push(value);
    fec_.Push(value);
  }

  void PushWordWithoutEncoding(uint16_t value) {
    bbu_.PushWord(value);
  }

  WOutStream& operator<<(const EncodeOut& encode) {
    PushWord(encode.GetValue());
    return *this;
  }

  WOutStream& operator<<(const NoEncodeOut& encode) {
    PushWordWithoutEncoding(encode.GetValue());
    return *this;
  }

  template <class Packet>
  WOutStream& operator<<(const PreambleOutImpl<Packet>& preamble) {
    for (size_t i = 0; i < preamble.kNumPreambleWords; ++i) {
      PushWordWithoutEncoding(preamble.kPreamble);
    }
    PushWordWithoutEncoding(preamble.kStartWord);
    return *this;
  }

  bool Valid() { return true; }
  operator bool() { return Valid(); }

private:
  BBU &bbu_;
  CRC &crc_;
  FEC &fec_;
};

#endif
