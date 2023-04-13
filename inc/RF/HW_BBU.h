#ifndef _LIBSPINE_BASE_BAND_UNIT_H_
#define _LIBSPINE_BASE_BAND_UNIT_H_

#include <BBU.h>
#include <SystemClock.h>
#include "IBBU.h"
#include "Utility.h"

template <BBU_name name>
class BaseBandUnit : public IBaseBandUnit<BaseBandUnit<name> > {
public:
  BaseBandUnit(uint16_t brg, uint16_t cfg0, uint16_t start_word = 0,
               uint16_t cfg1 = 0) {
    bbu_setup(brg, cfg0, cfg1, start_word, name);
  }

  void LockTiming() { bbu_lock_timing(name); }
  void UnlockTiming() { bbu_unlock_timing(name); }
  bool ContainsPreamble() { return bbu_contains_preamble(name); }
  uint16_t PeekWord() { return bbu_peek(name); }
  uint16_t GetWord() { return bbu_get_word(name); }
  void PushWord(uint16_t value) { bbu_push_word(value, name); }
  void SetTransmitting(bool transmitting) {
    if (transmitting) {
      bbu_set_transmitting(name);
    } else {
      bbu_set_receiving(name);
    }
  }
  uint16_t GetTime() const { return bbu_get_time(name); }
};

constexpr uint16_t GetSysClkDiv8(enum sys_freq sf) {
  static_assert(kNumSlicesPerMs == 16, "Formula for sysclk_div8 is now wrong!");
  return sys_clock_ticks_per_ms_slice(sf) * 2;
}

/* Desired baud rate must be expressed in kHz */
constexpr uint16_t kHzToBbuBrg(enum sys_freq sf, uint16_t br) {
  return static_cast<uint16_t>((float)65536 * (float)br / GetSysClkDiv8(sf)) -
         1;
}

constexpr uint16_t GetBbuBitStreamCfg(bool bidirectional = true) {
  return BBU_ENABLE | BBU_TX_ENABLE | BBU_ROM_ENABLE |
         ((bidirectional) ? BBU_BIDIRECTION_ENABLE : 0);
}

constexpr uint16_t GetBbuSlowClockCfg() {
  return BBU_ENABLE;
}

#endif
