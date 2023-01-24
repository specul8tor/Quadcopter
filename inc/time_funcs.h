/* Author: Thia Wyrod
 * License: GPLv3
 */
#ifndef _TIME_FUNCS_H_
#define _TIME_FUNCS_H_
#include <SystemClock.h>

#ifdef __BOARD_GINGER__
const enum crystal_freq kCrysF = crys_24_576_MHz;
#else
const enum crystal_freq kCrysF = crys_12_288_MHz;
#endif
const enum sys_freq kSystemF = _49_152_MHz;

static inline void init_clock(void)
{
  sys_clock_init(kCrysF, kSystemF);
}

static inline void wait_ms(size_t ms)
{
  for (size_t i = 0; i < ms; ++i) {
    for (size_t i = 0; i < kNumSlicesPerMs; ++i)
      sys_clock_wait(sys_clock_ticks_per_ms_slice(kSystemF));
  }
}

static inline void wait_ms2(size_t ms)
{
  for (size_t i = 0; i < ms; ++i) {
    for (size_t i = 0; i < 4; ++i)
      sys_clock_wait(sys_clock_ticks_per_ms_slice(kSystemF));
  }
}

static inline void wait_ms3(size_t ms)
{
  for (size_t i = 0; i < ms; ++i) {
    for (size_t i = 0; i < kNumSlicesPerMs; ++i)
      sys_clock_wait(sys_clock_ticks_per_ms_slice(kSystemF));
  }
}
#endif //_TIME_FUNCS_H_
