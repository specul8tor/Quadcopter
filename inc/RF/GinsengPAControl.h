#ifndef SKAA_COMMON_INC_WIRELESS_GINSENG_PA_CONTROL_H_
#define SKAA_COMMON_INC_WIRELESS_GINSENG_PA_CONTROL_H_

#include <cstddef>
#include <cstdint>

#include <IPort.h>

#include "IPAControl.h"

class GinsengPAControl : public IPAControl<GinsengPAControl> {
public:
  // Port E controls various radio GPIO, the following are all pins on Port E
  static constexpr size_t ChipSelectPin = 0, RxOnPin = 1, RxNotOnPin = 2,
                          RxOnPaVaPc0Pin = 3, RxOnPaVaPc1Pin = 4,
                          RxOnVpd0Pin = 5, RxOnVpd1Pin = 6;

  // Define a mask that represents the PA control pins
  static constexpr uint16_t PA_GPIO_MASK =
      GetGpioMask(RxOnPaVaPc0Pin, RxOnPaVaPc1Pin, RxOnVpd0Pin, RxOnVpd1Pin);

  // Do not mask off the lower 8 bits.  The general control mechanism is to use
  // pull downs when set as an input to set 0, and to drive high to set 1
  // Define some common masks using the PA_GPIO_MASK for common cases of biasing
  // the Power Amplifier for the A7125
  static constexpr uint16_t UPPER_POW_MASK =
      (GetGpioMask(RxOnVpd1Pin, RxOnPaVaPc1Pin) << PORT_CONFIG_OFFSET) |
      PA_GPIO_MASK;
  static constexpr uint16_t MID_POW_MASK =
      (GetGpioMask(RxOnVpd0Pin, RxOnPaVaPc0Pin) << PORT_CONFIG_OFFSET) |
      PA_GPIO_MASK;
  static constexpr uint16_t LOWER_POW_MASK =
      (GetGpioMask(RxOnVpd0Pin) << PORT_CONFIG_OFFSET) | PA_GPIO_MASK;

  // Initialize with all pins set as Outputs, driven low except RxOn
  // and the chip select pin which are driven high
  static constexpr uint16_t CTRL_PORT_INIT =
      (GetGpioMask(RxOnVpd0Pin, RxOnVpd1Pin, RxOnPaVaPc0Pin, RxOnPaVaPc1Pin,
                   RxOnPin, RxNotOnPin, ChipSelectPin)
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
        UPPER_POW_MASK, // 4
        MID_POW_MASK,   // 5
        MID_POW_MASK,   // 6
        MID_POW_MASK,   // 7
        MID_POW_MASK,   // 8
        MID_POW_MASK,   // 9
        MID_POW_MASK,   // 10
        MID_POW_MASK,   // 11
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
        0x27,
        0x26,
        0x25,
        0x2C,
        0x24,
        0x24,
        0x2B,
        0x23,
        0x31,
        0x30,
        0x21,
        0x20,
        0x25,
        0x24,
        0x2A,
        0x22
    };
    return PowerScalingRegTable[power];
  }
};

#endif

