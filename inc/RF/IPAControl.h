#ifndef SKAA_COMMON_INC_WIRELESS_PA_CONTROL_H_
#define SKAA_COMMON_INC_WIRELESS_PA_CONTROL_H_

// Defines an interface for PA Control in Eleven Engineering SIPs

template <class Derived> class IPAControl {
public:
  template <class... Pins> static constexpr uint16_t GetGpioMask(Pins... pins) {
    return GetGpioMaskImpl(0, pins...);
  }

  // Get the register value to program into the radio for a given power level
  static constexpr uint16_t GetPowerScalingReg(size_t power) {
    return Derived::GetPowerScalingRegImpl(power);
  }

  static constexpr uint16_t GetTransmitGpioConfig(uint16_t curr_cfg,
                                                  size_t power) {
    return Derived::GetTransmitGpioConfigImpl(curr_cfg, power);
  }

  static constexpr uint16_t GetReceiveGpioConfig(uint16_t curr_cfg) {
    return Derived::GetReceiveGpioConfigImpl(curr_cfg);
  }

private:
  static constexpr uint16_t GetGpioMaskImpl(uint16_t value) { return value; }
  template <class First, class... Rest>
  static constexpr uint16_t GetGpioMaskImpl(uint16_t value, First first_pin,
                                            Rest... rest) {
    return GetGpioMaskImpl(value | (1 << first_pin), rest...);
  }
};

#endif

