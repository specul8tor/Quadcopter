//
//  NoneRadio.h
//  This radio serves as a stub for platforms that do not have any radio control
//
//  Created by Sam Cristall
//
//

#ifndef LIBSPINE_COMMON_NONE_RADIO_H_
#define LIBSPINE_COMMON_NONE_RADIO_H_

static constexpr size_t INITIAL_PREAMBLE = 5;
static constexpr uint16_t PLL_TO_TX_TIME_IN_BBU_TICKS = 216;
static constexpr uint16_t RSSI_WAIT_TIME_IN_BBU_TICKS = 100;

struct StandardNoneRadioConfigParams {};

template <class Config, class PAControlTy, class SPIwCS, class BBUTy,
          class RadioPort, class BBUPinTy>
class NoneRadio
    : public IRadio<
          NoneRadio<Config, PAControlTy, SPIwCS, BBUTy, RadioPort, BBUPinTy>> {
public:
  using BBU = IBaseBandUnit<BBUTy>;

  NoneRadio(BBU &bbu) : bbu_(bbu) {}

  //! Change the rf channel of the A7196
  void ChangeChannel(size_t channel) {
    assert(channel <= 49 && "A7196 channel must be <= 49");
    bbu_.PushWord(0);
    bbu_.PushWord(0);
  }

  //! Put the radio and the bbu into transmit mode
  //! \note Will send PREAMBLE for 16 * INITIAL_PREAMBLE bits
  //! \warning Transmission must begin immediately after running or timing may
  //!          break down
  void SwitchToTransmit(size_t power_level) {
    assert(power_level <= 15 && "Power level must be <= 15");

    // Start a timer now, to start the countdown to poll cycle start
    auto timer = MakeSoftTimer(bbu_);
    timer.Start(PLL_TO_TX_TIME_IN_BBU_TICKS);

    // Let the timer elapse so that we are sure the radio is on and ready to go
    timer.Wait();

    // When we first strobe to Tx, we need to send a bunch of preamble to cal
    // the radio into a good state. More preamble will result in easier lock-on
    bbu_.SetTransmitting(true);
    for (size_t i = 0; i < INITIAL_PREAMBLE; ++i)
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

    bbu_.SetTransmitting(false);
  }

  //! Put the radio into low power sleep
  void SwitchToSleep() {}

  void SwitchToStandby() {}

  //! Get an RSSI sample from the radio -- this will take >100 bbu ticks
  //! \note The radio will be in PLL mode after exiting
  uint16_t GetRSSI() { return 0; }

  static void AlterConfigData(uint16_t, uint16_t, uint16_t) {}

private:
  BBU &bbu_;
};

template <class Cfg, class Pa, class S, class B, class Port, class Pin>
constexpr NoneRadio<Cfg, Pa, S, B, Port, Pin>
MakeNoneRadio(Cfg, Pa &, S &, B &bbu, Port &, Pin &) {
  return {bbu};
}

#endif
