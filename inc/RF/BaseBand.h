#ifndef LIBSPINE_COMMON_BASEBAND_H_
#define LIBSPINE_COMMON_BASEBAND_H_

#include "SoftTimer.h"
#include "WInStream.h"
#include "WOutStream.h"
#include "IBBU.h"

//! Encoder class stub for use in place of CRC or FEC when not using those
//! peripherals

enum class EncoderStrategy {
  Configure,
  Reset,
  Nothing,
};

//! Wireless class that offers creation of input and output streams to the XInC2
//! wireless base band
template <class BBUTy, class CRC = NullEncoder, class FEC = NullEncoder>
class BaseBand {
public:
  using BBU = IBaseBandUnit<BBUTy>;
  using WInTy = WInStream<BBUTy, CRC, FEC>;
  using WOutTy = WOutStream<BBUTy, CRC, FEC>;

  //! Constructor
  constexpr BaseBand(BBU &bbu, uint16_t start_word,
                     CRC &crc = NullEncoder::GetInstance(),
                     FEC &fec = NullEncoder::GetInstance())
      : bbu_(bbu), start_word_(start_word), crc_(crc), fec_(fec) {}

  //! Try to acquire base band input stream
  //! Note: only use lock_on in clock recovering, bidirectional mode!
  template <class ClockTy>
  WInTy
  TryAcquireInputStream(const ClockTy &timer,
                        EncoderStrategy enc_sgy = EncoderStrategy::Configure) {
    // By unlocking the timing, we enable lots of fun stuff in the BBU like
    // timing recovery and preamble detection!
    bbu_.UnlockTiming();

    do {
      while (!bbu_.ContainsPreamble()) {
        if (timer.HasElapsed())
          return WInTy(bbu_, crc_, fec_, false);
      }
      while (bbu_.ContainsPreamble()) {
        if (timer.HasElapsed())
          return WInTy(bbu_, crc_, fec_, false);
      }
    } while (bbu_.PeekWord() != start_word_);

    bbu_.LockTiming();

    switch (enc_sgy) {
      case EncoderStrategy::Configure:
        crc_.ConfigureDecoder();
        fec_.ConfigureDecoder();
        break;
      case EncoderStrategy::Reset:
        crc_.Reset();
        fec_.Reset();
        break;
      default:
        break;
    }

    return WInTy(bbu_, crc_, fec_, true);
  }

  WOutTy TryAcquireOutputStream(
      EncoderStrategy enc_sgy = EncoderStrategy::Configure) {
    switch (enc_sgy) {
      case EncoderStrategy::Configure:
        crc_.ConfigureEncoder();
        fec_.ConfigureEncoder();
        break;
      case EncoderStrategy::Reset:
        crc_.Reset();
        fec_.Reset();
        break;
      default:
        break;
    }
    return WOutTy(bbu_, crc_, fec_);
  }

  //! Try to acquire base band output stream
  template <class ClockTy>
  WOutTy
  TryAcquireOutputStream(const ClockTy &timer,
                         EncoderStrategy enc_sgy = EncoderStrategy::Configure) {
    while (timer.HasElapsed() == false)
      continue;

    return TryAcquireOutputStream(enc_sgy);
  }

private:
  BBU &bbu_;
  uint16_t start_word_;
  CRC& crc_;
  FEC& fec_;
};

template <class B, class ...Args>
constexpr BaseBand<B, Args...>
MakeBaseBand(B &bbu, uint16_t start_word, Args&... args) {
  return {bbu, start_word, args...};
}

#endif //LIBSPINE_COMMON_BASEBAND_H_

