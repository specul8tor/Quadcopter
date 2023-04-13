#ifndef SKAA_COMMON_INC_WIRELESS_SAFFRON_PA_CONTROL_H_
#define SKAA_COMMON_INC_WIRELESS_SAFFRON_PA_CONTROL_H_

#include <cstddef>
#include <cstdint>

#include <IPort.h>

#include "IPAControl.h"

class SaffronPAControl : public IPAControl<SaffronPAControl> {
public:
  // Port E controls various radio GPIO, the following are all pins on Port E
  static constexpr size_t ChipSelectPin = 0, RxOnPin = 1, RxNotOnPin = 2,
                          PaEnHp = 3, PaEnMp = 4, PaEnLp = 5;

  // Define a mask that represents the PA control pins
  static constexpr uint16_t PA_GPIO_MASK = GetGpioMask(PaEnHp, PaEnMp, PaEnLp);

  // Do not mask off the lower 8 bits.  The general control mechanism is to use
  // pull downs when set as an input to set 0, and to drive high to set 1
  // Define some common masks using the PA_GPIO_MASK for common cases of biasing
  // the Power Amplifier for the A7125
  static constexpr uint16_t UPPER_POW_MASK =
      (GetGpioMask(PaEnHp, PaEnMp) << PORT_CONFIG_OFFSET) | PA_GPIO_MASK;
  static constexpr uint16_t MID_POW_MASK =
      (GetGpioMask(PaEnMp) << PORT_CONFIG_OFFSET) | PA_GPIO_MASK;
  static constexpr uint16_t LOWER_POW_MASK =
      (GetGpioMask(PaEnLp) << PORT_CONFIG_OFFSET) | PA_GPIO_MASK;

  // Initialize with all pins set as Outputs, driven low except RxOn
  // and the chip select pin which are driven high
  static constexpr uint16_t CTRL_PORT_INIT =
      (GetGpioMask(PaEnHp, PaEnMp, PaEnLp, RxOnPin, RxNotOnPin, ChipSelectPin)
       << PORT_CONFIG_OFFSET) |
      (GetGpioMask(RxOnPin, ChipSelectPin));


  static uint16_t GetTransmitGpioConfigImpl(uint16_t curr_cfg,
                                                      size_t power) {
    // Group the PA mask entries into a lookup table
    static constexpr uint16_t PowerScalingGpioTable[] = {
        UPPER_POW_MASK, // 0
        UPPER_POW_MASK, // 1
        UPPER_POW_MASK, // 2
        UPPER_POW_MASK, // 3
        MID_POW_MASK,   // 4
        MID_POW_MASK,   // 5
        MID_POW_MASK,   // 6
        MID_POW_MASK,   // 7
        MID_POW_MASK,   // 8
        LOWER_POW_MASK, // 9
        LOWER_POW_MASK, // 10
        LOWER_POW_MASK, // 11
        LOWER_POW_MASK, // 12
        LOWER_POW_MASK, // 13
        LOWER_POW_MASK, // 14
        LOWER_POW_MASK  // 15
    };
    return (curr_cfg & (~(1 << RxOnPin) & ~(PA_GPIO_MASK << 8))) |
           ((1 << RxNotOnPin) | PowerScalingGpioTable[power]);
  }

  static constexpr uint16_t GetReceiveGpioConfigImpl(uint16_t) {
    return CTRL_PORT_INIT;
  }

  // Get the register value to program into the radio for a given power level
  static uint16_t GetPowerScalingRegImpl(size_t power) {
    static constexpr uint16_t PowerScalingRegTable[] = {
        0x27, // 0 -- Max Power
        0x26, // 1
        0x2D, // 2
        0x25, // 3
        0x26, // 4
        0x2D, // 5
        0x25, // 6
        0x2C, // 7
        0x24, // 8
        0x24, // 9
        0x2B, // 10
        0x23, // 11
        0x2A, // 12
        0x22, // 13
        0x21, // 14
        0x20, // 15 -- Min Power
    };
    return PowerScalingRegTable[power];
  }
};

#endif


