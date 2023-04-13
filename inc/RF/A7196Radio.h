//
//  A7196Radio.h
//  skaa
//
//  Created by Sam Cristall
//
//

#ifndef LIBSPINE_COMMON_A7196_RADIO_H_
#define LIBSPINE_COMMON_A7196_RADIO_H_

#include "A7196RadioData.h"
#include "IBBU.h"
#include "IPAControl.h"
#include "IRadio.h"
#include "ISPI.h"
#include "OutputPin.h"
#include "SoftTimer.h"
#include "WirelessConsts.h"

struct Standard2MbitA7196ConfigParams {
  // This is PH0, which switches in the receive or transmit BBU paths
  static constexpr size_t BBU_PIN_NUMBER = 2;

  static constexpr size_t CALIBRATION_CHANNEL_LOW = 30;
  static constexpr size_t CALIBRATION_CHANNEL_MID = 90;
  static constexpr size_t CALIBRATION_CHANNEL_HIGH = 150;

  static constexpr size_t INITIAL_PREAMBLE = 5;
  static constexpr uint16_t PLL_TO_TX_TIME_IN_BBU_TICKS = 216;
  static constexpr uint16_t RSSI_WAIT_TIME_IN_BBU_TICKS = 100;

  static constexpr DataRate DATA_RATE = DataRate::_2048_Mbit;

  using RegisterData = A7196_2Mbit_Data;
};

struct Standard4MbitA7196ConfigParams {
  // This is PH0, which switches in the receive or transmit BBU paths
  static constexpr size_t BBU_PIN_NUMBER = 2;

  static constexpr size_t CALIBRATION_CHANNEL_LOW = 30;
  static constexpr size_t CALIBRATION_CHANNEL_MID = 90;
  static constexpr size_t CALIBRATION_CHANNEL_HIGH = 150;

  static constexpr size_t INITIAL_PREAMBLE = 20;
  static constexpr uint16_t PLL_TO_TX_TIME_IN_BBU_TICKS = 432;
  static constexpr uint16_t RSSI_WAIT_TIME_IN_BBU_TICKS = 200;

  static constexpr DataRate DATA_RATE = DataRate::_4096_Mbit;

  using RegisterData = A7196_4Mbit_Data;
};

template <class ConfigParams, class PAControlTy, class SPIwCS, class BBUTy,
          class RadioPort, class BBUPinTy>
class A7196Radio : public IRadio<A7196Radio<ConfigParams, PAControlTy, SPIwCS,
                                            BBUTy, RadioPort, BBUPinTy>> {
public:
  // Strobe commands are how the A7196 moves from mode to mode.  See the
  // A7196 datasheet for more description.  We only really use PLL/Rx/Tx
  static constexpr uint16_t StrobeSleep = 0x80, StrobeIdle = 0x90,
                            StrobeStandby = 0xA0, StrobePLL = 0xB0,
                            StrobeRx = 0xC0, StrobeTx = 0xD0, StrobeTFR = 0xE0,
                            StrobeRFR = 0xF0;

  using PAControl = IPAControl<PAControlTy>;
  using BBU = IBaseBandUnit<BBUTy>;
  using BBUPin = IOutputPin<BBUPinTy>;

  A7196Radio(PAControl &pa_control, SPIwCS &spi, BBU &bbu)
      : pa_control_(pa_control), spi_(spi), bbu_(bbu) {

    // set the radio control pin low
    RadioPort::Configure((RadioPort::Status() | (1 << 10)) & ~(1 << 2));

    // Configure/calibrate the radio
    ConfigureRadio();
  }

  //! Change the rf channel of the A7196
  void ChangeChannel(size_t channel) {
    assert(channel <= 49 && "A7196 channel must be <= 49");
    bbu_.PushWord(0);
    bbu_.PushWord(0);
    spi_.Push(StrobePLL);
    // Use a simple scaling factor + offset to give good channel separation
    // Unsure how this was derived -- likely empirically a long long time ago
    WriteReg(A7196Reg::PLL1, (channel * 3) + 7);
  }

  //! Put the radio and the bbu into transmit mode
  //! \note Will send PREAMBLE for 16 * INITIAL_PREAMBLE bits
  //! \warning Transmission must begin immediately after running or timing may
  //!          break down
  void SwitchToTransmit(size_t power_level) {
    assert(power_level <= 15 && "Power level must be <= 15");

    // Put the radio into Tx Mode
    spi_.Push(StrobePLL);
    spi_.Push(StrobeTx);

    // Start a timer now, to start the countdown to poll cycle start
    auto timer = MakeSoftTimer(bbu_);
    timer.Start(ConfigParams::PLL_TO_TX_TIME_IN_BBU_TICKS);

    // Get the power level and program it into the A7196
    auto a7196_power_setting = pa_control_.GetPowerScalingReg(power_level);
    auto tlc = a7196_power_setting >> 8;
    WriteReg(A7196Reg::TXTEST, a7196_power_setting);
    // Write tlc to ROMP6 by first setting the page in RFANALOG
    WriteReg(A7196Reg::RFANALOG, 6 << 4);
    WriteReg(A7196Reg::ROMP_REG, tlc << 4);

    // Let the timer elapse so that we are sure the radio is on and ready to go
    timer.Wait();

    // When we first strobe to Tx, we need to send a bunch of preamble to cal
    // the radio into a good state. More preamble will result in easier lock-on
    bbu_.SetTransmitting(true);
    _outp(_inp(GPHcfg) & ~(1 << 8), GPHcfg);
    _outp((_inp(BBU0cfg0) | (1 << 13) | (1 << 5)) & ~(1 << 7), BBU0cfg0);
    for (size_t i = 0; i < ConfigParams::INITIAL_PREAMBLE; ++i)
      bbu_.PushWord(PREAMBLE);
  }

  //! Turn off transmission, but do not affect radio state
  void TurnOffTransmit(bool with_flush) {
    if (with_flush) {
      // Flush the BBU
      bbu_.PushWord(0);
      bbu_.PushWord(0);
    }
  }

  //! Put the radio and the bbu into receive mode
  void SwitchToReceive() {
    TurnOffTransmit(false);

    // Set the BBU direction pin to low, indicating input
    spi_.Push(StrobePLL);
    spi_.Push(StrobeRx);
    bbu_.SetTransmitting(false);
  }

  void SwitchToStandby() {
    SwitchToSleep();
  }

  //! Put the radio into low power sleep
  void SwitchToSleep() {
    SwitchToReceive();

    spi_.Push(StrobeSleep);
  }

  //! Get an RSSI sample from the radio -- this will take >100 bbu ticks
  //! \note The radio will be in PLL mode after exiting
  uint16_t GetRSSI() {
    WriteReg(A7196Reg::MODECTRL, A7196_MODECTRL_MEASURE_ADC);
    WriteReg(A7196Reg::MODECTRL, A7196_MODECTRL_MEASURE_RSSI);
    spi_.Push(StrobeRx);
    while (ReadReg(A7196Reg::MODECTRL) & 1)
      continue;
    auto timer = MakeSoftTimer(bbu_);
    timer.Start(ConfigParams::RSSI_WAIT_TIME_IN_BBU_TICKS);
    timer.Wait();
    WriteReg(A7196Reg::MODECTRL, A7196_MODECTRL_AUTO_IF_OFFSET);
    uint16_t result = ReadReg(A7196Reg::MODECTRL);
    spi_.Push(StrobePLL);
    return result;
  }

  // For use in test setups, alter the base config data for the next time
  // the radio is configured
  static void AlterConfigData(uint16_t, uint16_t, uint16_t) {}

private:
  template <class... Args> void WriteReg(A7196Reg reg, Args... vals) {
    spi_.Push(static_cast<uint16_t>(reg), vals...);
  }

  uint16_t ReadReg(A7196Reg reg) {
    // To read a reg on the A7196, you set bit 6
    return spi_.GetByte(static_cast<uint16_t>(reg) | (1 << 6));
  }

  void ConfigureRadio() {
    // This will reset the radio, preparing it for configuration
    WriteReg(A7196Reg::MODE, A7196_MODE_INITIAL);

    WriteReg(A7196Reg::IDCODE, 0x55, 0x55, 0x55, 0x55);

    // Loop through the config data, and if it doesn't equal DONT_CONFIG,
    // write the reg
    for (size_t i = 0; i < ConfigParams::RegisterData::Base.size(); ++i) {
      uint16_t value = ConfigParams::RegisterData::Base[i];
      if (value != A7196_REG_DONT_CONFIG)
        WriteReg(static_cast<A7196Reg>(i), value);
    }

    for (size_t i = 0; i < ConfigParams::RegisterData::Addr2A.size(); ++i) {
      WriteReg(A7196Reg::RFANALOG, i << 4);
      WriteReg(A7196Reg::DASP_REG, ConfigParams::RegisterData::Addr2A[i]);
    }

    for (size_t i = 0; i < ConfigParams::RegisterData::Addr38.size(); ++i) {
      WriteReg(A7196Reg::RFANALOG, i << 4);
      WriteReg(A7196Reg::ROMP_REG, ConfigParams::RegisterData::Addr38[i]);
    }

    // Write the calibration reg to do initial calibrations
    spi_.Push(StrobePLL);
    WriteReg(A7196Reg::CALIBRATION, A7196_CALIBRATION_INITAL);
    while (ReadReg(A7196Reg::CALIBRATION) & A7196_CALIBRATION_INITAL)
      continue;

    // We do a bit of repitition here -- we will set up the PLL to a channel,
    // then calibrate it, then assert it's okay
    // declare a lambda to reduce local repetition
    auto CHGroupCal = [&](size_t channel) {
      WriteReg(A7196Reg::PLL1, channel);
      WriteReg(A7196Reg::CALIBRATION, A7196_CALIBRATION_CHANNEL);
      while (ReadReg(A7196Reg::CALIBRATION) & A7196_CALIBRATION_CHANNEL)
        continue;

      assert(!(ReadReg(A7196Reg::VCOCAL1) & (1 << 3)) &&
             !(ReadReg(A7196Reg::VCOCCAL) & (1 << 4)) && "Failed to cal A7196");
    };

    // Check the various channel groups
    CHGroupCal(ConfigParams::CALIBRATION_CHANNEL_LOW);
    CHGroupCal(ConfigParams::CALIBRATION_CHANNEL_MID);
    CHGroupCal(ConfigParams::CALIBRATION_CHANNEL_HIGH);

    // Put the radio into standby
    spi_.Push(StrobeStandby);

    // This should never happen, but good for debugging broken hardware
    assert(!(ReadReg(A7196Reg::IFCAL1) & (1 << 4)) && "Failed to cal A7196");

    // Read these out to clear their calibration registers
    // but discard the data
    // TODO: We should check if we need to assert these too
    ReadReg(A7196Reg::IFCAL2);
    ReadReg(A7196Reg::VCOCAL2);
  }

  PAControl &pa_control_;
  SPIwCS &spi_;
  BBU &bbu_;
};

template <class Cfg, class Pa, class S, class B, class Port, class Pin>
constexpr A7196Radio<Cfg, Pa, S, B, Port, Pin>
MakeA7196Radio(Cfg, Pa &pa, S &spi, B &bbu, Port &, Pin &) {
  return {pa, spi, bbu};
}

#endif
