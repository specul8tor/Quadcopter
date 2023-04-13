#ifndef LIBSPINE_COMMON_WIRELESS_CONSTS_H
#define LIBSPINE_COMMON_WIRELESS_CONSTS_H
#include <cstdint>

enum class DataRate {
  _2048_Mbit,
  _4096_Mbit,
};

// Radio is 2.048 MHz, and this setting will give us 2.048062.5
// 2048062.5 = 49152000 / 8 / 65536 * ({21845} + 1)
static const uint16_t kRadioBbuBaudRate = 21845;
static const uint16_t kRadioStartWord = 0x217B;
static const uint16_t kRadioPreamble = 0x5555;
static const uint16_t kRadioTimeout = 200;

#endif
