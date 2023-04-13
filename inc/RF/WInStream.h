#ifndef LIBSPINE_COMMON_W_IN_STREAM_H_
#define LIBSPINE_COMMON_W_IN_STREAM_H_

#include "IBBU.h"
#include "FEC_CRC.h"

// EncodeIn proxy class for streaming
//   win >> EncodeIn(in_var);   // reads from BBU and pumps FEC/CRC LFSRs
//   win >> NoEncodeIn(in_var); // only reads from BBU
template <bool Encode>
class EncodeInProxy {
public:
  constexpr EncodeInProxy(uint16_t& value) : value_(value) {}

  void SetValue(uint16_t value) { value_ = value; }

private:
  uint16_t& value_;
};

using EncodeIn = EncodeInProxy<true>;
using NoEncodeIn = EncodeInProxy<false>;

class WInResults {
public:
  constexpr WInResults(bool valid, uint16_t crc_word, uint16_t fec_syndrome)
   : Valid(valid), CrcWord(crc_word), FecSyndrome(fec_syndrome) {}

  operator bool() const {
    return Valid;
  }

   const bool Valid;
   const uint16_t CrcWord;
   const uint16_t FecSyndrome;
private:
};

template <typename BBUTy, typename CRC, typename FEC> class WInStream {
public:
  using BBU = IBaseBandUnit<BBUTy>;

  WInStream(BBU &bbu, CRC &crc, FEC &fec, bool valid)
      : bbu_(bbu), crc_(crc), fec_(fec), valid_(valid) {}

  uint16_t GetWordWithNoCrcOrFec() {
    return GetWord(false, false);
  }

  uint16_t GetWordWithCrc() {
    return GetWord(true, false);
  }

  uint16_t GetWordWithCrcAndFec() {
    return GetWord(true, true);
  }

  void PushCrc(uint16_t word) {
    crc_.Push(word);
  }

  void PushFec(uint16_t word) {
    fec_.Push(word);
  }

  template <typename ...Words>
  bool CheckCrc(Words... words) {
    crc_.Push(words...);
    return crc_.Get() == 0;
  }

  bool CheckCrc() {
    return crc_.Get() == 0;
  }

  bool Valid() { return valid_; }
  operator bool() { return Valid(); }

  template <typename ...Data>
  uint16_t FinalizeFec(uint16_t fec_keyword, Data... data) {
    FinalizeFecImpl(data...);
    return fec_.FinalizeDecode(fec_keyword);
  }

  WInStream& operator>>(EncodeIn&& encode) {
    encode.SetValue(GetWordWithCrcAndFec());
    return *this;
  }

  WInStream& operator>>(NoEncodeIn&& encode) {
    encode.SetValue(GetWordWithNoCrcOrFec());
    return *this;
  }


private:
  template <typename ...Rest>
  void FinalizeFecImpl(uint16_t first_word, Rest... rest) {
    fec_.Push(first_word);
    FinalizeFecImpl(rest...);
  }

  void FinalizeFecImpl() {}

  uint16_t GetWord(bool enable_crc, bool enable_fec) {
    uint16_t word = bbu_.GetWord();
    if (enable_crc) crc_.Push(word);
    if (enable_fec) fec_.Push(word);
    return word;
  }

  BBU &bbu_;
  CRC &crc_;
  FEC &fec_;
  bool valid_;
};
#endif
