#ifndef SKAA_COMMON_INC_WIRELESS_SAGE_PA_CONTROL_H_
#define SKAA_COMMON_INC_WIRELESS_SAGE_PA_CONTROL_H_

#include <cstddef>
#include <cstdint>

#include "IPAControl.h"

class SagePAControl : public IPAControl<SagePAControl> {
public:
  // These are not used on Sage
  static uint16_t GetTransmitGpioConfigImpl(uint16_t, size_t) {
    return 0;
  }

  // These are not used on Sage
  static constexpr uint16_t GetReceiveGpioConfigImpl(uint16_t) {
    return 0;
  }

  // Get the register value to program into the radio for a given power level
  static uint16_t GetPowerScalingRegImpl(size_t power) {
    static constexpr uint16_t PowerScalingRegTable[] = {
        // Format:
        // TLC   | LowPow   |    PA    |  Driver  | Buffer
        // 3-bit | 1-bit    |    2-bit |  3-bit   | 2-bit 
        (0 << 8) | (0 << 7) | (3 << 5) | (7 << 2) | (3), // 0
        (1 << 8) | (1 << 7) | (3 << 5) | (6 << 2) | (1), // 1
        (2 << 8) | (1 << 7) | (2 << 5) | (7 << 2) | (0), // 2
        (7 << 8) | (1 << 7) | (1 << 5) | (6 << 2) | (2), // 3
        (2 << 8) | (1 << 7) | (2 << 5) | (3 << 2) | (0), // 4
        (1 << 8) | (1 << 7) | (2 << 5) | (0 << 2) | (2), // 5
        (3 << 8) | (1 << 7) | (1 << 5) | (4 << 2) | (0), // 6
        (3 << 8) | (1 << 7) | (2 << 5) | (2 << 2) | (0), // 7
        (1 << 8) | (1 << 7) | (2 << 5) | (0 << 2) | (0), // 8
        (5 << 8) | (0 << 7) | (0 << 5) | (0 << 2) | (0), // 9
        (2 << 8) | (1 << 7) | (0 << 5) | (0 << 2) | (0), // 10
        (6 << 8) | (1 << 7) | (1 << 5) | (0 << 2) | (0), // 11
        (5 << 8) | (1 << 7) | (1 << 5) | (0 << 2) | (0), // 12
        (6 << 8) | (1 << 7) | (0 << 5) | (0 << 2) | (0), // 13
        (3 << 8) | (1 << 7) | (0 << 5) | (0 << 2) | (0), // 14
        (7 << 8) | (1 << 7) | (0 << 5) | (0 << 2) | (0), // 15
    };

    return PowerScalingRegTable[power];
  }
};

#endif



