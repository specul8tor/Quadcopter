//
//  RT569DigFPGA.h
//  Driver for FPGA emulation 
//  of the RT569 digital baseband
//
//  Created by Sam Cristall
//  and Ian Thomson kinda
//

#ifndef LIBSPINE_COMMON_RT569DIG_RADIO_H_
#define LIBSPINE_COMMON_RT569DIG_RADIO_H_

#include "IBBU.h"
#include "IPAControl.h"
#include "IRadio.h"
#include "ISPI.h"
#include "OutputPin.h"
#include "SoftTimer.h"
#include "WirelessConsts.h"
#include "XpdStream.h"

struct StandardRT569DigConfigParams {
  static constexpr size_t INITIAL_PREAMBLE = 5;
  static constexpr uint16_t PLL_TO_TX_TIME_IN_BBU_TICKS = 216;
  static constexpr uint16_t RSSI_WAIT_TIME_IN_BBU_TICKS = 100;

  static constexpr bool INIT_RF = false;
  static constexpr bool INIT_PMU = false;
  static constexpr bool INIT_MODEM = false;
};

struct RT569DigConfigWithFullInitParams {
  static constexpr size_t INITIAL_PREAMBLE = 5;
  static constexpr uint16_t PLL_TO_TX_TIME_IN_BBU_TICKS = 216;
  static constexpr uint16_t RSSI_WAIT_TIME_IN_BBU_TICKS = 100;

  static constexpr bool INIT_RF = true;
  static constexpr bool INIT_PMU = true;
  static constexpr bool INIT_MODEM = true;
};

struct RT569DigConfigWithModemInitParams {
  static constexpr size_t INITIAL_PREAMBLE = 5;
  static constexpr uint16_t PLL_TO_TX_TIME_IN_BBU_TICKS = 216;
  static constexpr uint16_t RSSI_WAIT_TIME_IN_BBU_TICKS = 100;

  static constexpr bool INIT_RF = false;
  static constexpr bool INIT_PMU = false;
  static constexpr bool INIT_MODEM = true;
};

namespace RT569DigDetail {

static constexpr uint16_t kModemDataBaseAddress = 0x200;
static constexpr std::array<std::array<uint16_t, 2>, 56> kModemInitData = {{
    {{0x0001, 0x0200}}, {{0x0200, 0x0140}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x7f00, 0x0000}},
    {{0x0000, 0x007f}}, {{0x007f, 0x007f}}, {{0x0081, 0x0200}},
    {{0x0320, 0x3f00}}, {{0x7176, 0x4129}}, {{0x4516, 0x1000}},
    {{0x0020, 0x0320}}, {{0x0205, 0x4321}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0805, 0x02ff}}, {{0x1110, 0x0e0b}}, {{0x0000, 0x0040}},
    {{0x0258, 0x0015}}, {{0x0000, 0x0100}}, {{0x1e04, 0x0040}},
    {{0x2308, 0x0320}}, {{0x0000, 0x0200}}, {{0x000f, 0x0001}},
    {{0x0002, 0x0700}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0xfe00, 0x0102}}, {{0xfdfc, 0xfcfd}},
    {{0x0a06, 0x02ff}}, {{0x1413, 0x110e}}
}};

static constexpr uint16_t kRfDataBaseAddress = 0x300;
static constexpr std::array<std::array<uint16_t, 2>, 0> kRfInitData = {{
    //No RF params in the FPGA
}};

static constexpr uint16_t kPmuDataBaseAddress = 0x400;
static constexpr std::array<std::array<uint16_t, 2>, 0> kPmuInitData = {{
    //No PMU either except a few random regs
}};

static constexpr std::array<std::array<uint16_t, 2>, 49> channel_settings = {{
    {{0, 18946}}, {{0, 22018}}, {{0, 25090}}, {{0, 28162}}, {{0, 31234}},
    {{0, 34306}}, {{0, 37378}}, {{0, 40450}}, {{0, 43522}}, {{0, 46594}},
    {{0, 49666}}, {{0, 52738}}, {{0, 55810}}, {{0, 58882}}, {{0, 61954}},
    {{0, 65026}}, {{1, 2562}},  {{1, 5634}},  {{1, 8706}},  {{1, 11778}},
    {{1, 14850}}, {{1, 17922}}, {{1, 20994}}, {{1, 24066}}, {{1, 27138}},
    {{1, 30210}}, {{1, 33282}}, {{1, 36354}}, {{1, 39426}}, {{1, 42498}},
    {{1, 45570}}, {{1, 48642}}, {{1, 51714}}, {{1, 54786}}, {{1, 57858}},
    {{1, 60930}}, {{1, 64002}}, {{2, 1538}},  {{2, 4610}},  {{2, 7682}},
    {{2, 10754}}, {{2, 13826}}, {{2, 16898}}, {{2, 19970}}, {{2, 23042}},
    {{2, 26114}}, {{2, 29186}}, {{2, 32258}}, {{2, 35330}},
}};

static constexpr std::array<uint16_t, 16> power_settings = {{
    59,
    57,
    54,
    52,
    50,
    47,
    45,
    43,
    40,
    38,
    36,
    33,
    31,
    29,
    26,
    24,
}};

static constexpr std::array<uint16_t, 16> lna_gain_codes = {{
    0,
    2,
    4,
    6,
    7,
    8,
    10,
    13,
    12,
    13,
    16,
    18,
    21,
    22,
    24,
    24,
}};
static constexpr std::array<uint16_t, 16> tia_gain_codes = {{
    0,
    2,
    6,
    8,
    8,
    11,
    12,
    15,
    18,
    22,
    25,
    28,
    31,
    34,
    38,
    41,
}};
static constexpr std::array<uint16_t, 16> vga_gain_codes = {{
    0,
    3,
    6,
    6,
    6,
    6,
    10,
    12,
    15,
    18,
    21,
    24,
    27,
    30,
    30,
    30,
}};

static constexpr std::array<std::array<uint16_t, 3>, 3> init_a_la_cart = {{
    {{0x438 >> 2, 0x9010, 0x0000}}, // Random PMU settings iunno
    {{0x43C >> 2, 0x0000, 0xE0FF}},
    {{0x450 >> 2, 0x0000, 0x0003}},
}};

static constexpr std::array<std::array<uint16_t, 3>, 10> settings_2048_mode = {{
    {{0x200 >> 2, 0x0000, 0x0400}}, // Tx_system
    {{0x220 >> 2, 0x0080, 0x0400}}, // Rx_system
    {{0x224 >> 2, 0x04B0, 0x3F02}}, // Rx_packet_sync
    {{0x22C >> 2, 0x1347, 0x2000}}, // Rx_CFO_tracking
    {{0x230 >> 2, 0x0028, 0x04B0}}, // Rx_timing_tracking
    {{0x248 >> 2, 0x0000, 0x0000}}, // Rx_FIR_coef_S1_C
    {{0x24C >> 2, 0x7F00, 0x0000}}, // Rx_FIR_coef_S1_D
    {{0x25C >> 2, 0x2104, 0x0040}}, // RX_Rest
    {{0x264 >> 2, 0x0000, 0x0100}}, // Tx_upsampling
    {{0x43C >> 2, 0x0000, 0xE0FF}}, // adda_slk_sel
}};
} // namespace RT569DigDetail

template <class Config, class PAControlTy, class SPIwCS, class BBUTy,
          class RadioPort, class BBUPinTy>
class RT569DigFPGA
    : public IRadio<
          RT569DigFPGA<Config, PAControlTy, SPIwCS, BBUTy, RadioPort, BBUPinTy>> {
public:
  using BBU = IBaseBandUnit<BBUTy>;
  using BBUPin = IOutputPin<BBUPinTy>;

  RT569DigFPGA(SPIwCS &spi, BBU &bbu) : spi_(spi), bbu_(bbu) {

    // Configure/calibrate the radio
    ConfigureRadio();
  }

  void ConfigureRadioPlease() {
    ConfigureRadio();
  }

  //! Change the rf channel of the RT569
  void ChangeChannel(size_t channel) {
    assert(channel <= 49 && "RT569 channel must be <= 49");
    bbu_.PushWord(0);
    bbu_.PushWord(0);
    //Channels aren't real on FPGA
    WriteReg(0x300 >> 2, (RT569DigDetail::channel_settings[channel][0] << 6) | 15,
             RT569DigDetail::channel_settings[channel][1]);
  }

  //! Put the radio and the bbu into transmit mode
  //! \note Will send PREAMBLE for 16 * INITIAL_PREAMBLE bits
  //! \warning Transmission must begin immediately after running or timing may
  //!          break down
  void SwitchToTransmit(size_t power_level) {
    assert(power_level <= 15 && "Power level must be <= 15");

    // Start a timer now, to start the countdown to poll cycle start
    auto timer = MakeSoftTimer(bbu_);
    timer.Start(Config::PLL_TO_TX_TIME_IN_BBU_TICKS);
    
    //Don't write these regs in FPGA
    //WriteReg(0x3A8 >> 2, 0x0000, RT569DigDetail::power_settings[power_level]);
    //switch to tx tho
    WriteReg(0x450 >> 2, 0x0000, 0x0103);

    // Let the timer elapse so that we are sure the radio is on and ready to go
    timer.Wait();

    // When we first strobe to Tx, we need to send a bunch of preamble to cal
    // the radio into a good state. More preamble will result in easier lock-on
    bbu_.SetTransmitting(true);
    _outp(_inp(BBU0cfg0) | (1 << 7), BBU0cfg0);
    for (size_t i = 0; i < Config::INITIAL_PREAMBLE; ++i)
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
    bbu_.SetTransmitting(false);
    //WriteReg(0x314 >> 2, 0x4000, 0x00CF); // Start AGC -- Not on FPGA
    WriteReg(0x450 >> 2, 0x0000, 0x0203);
  }

  //! Put the radio into low power sleep
  void SwitchToSleep() { WriteReg(0x450 >> 2, 0x0000, 0x0003); }

  //! Put the radio into low power sleep
  void SwitchToStandby() { WriteReg(0x450 >> 2, 0x0000, 0x0003); }

  //! Get an RSSI sample from the radio -- this will take >100 bbu ticks
  uint16_t GetRSSI() {
    SwitchToReceive();

    auto timer = MakeSoftTimer(bbu_);
    timer.Start(Config::RSSI_WAIT_TIME_IN_BBU_TICKS);
    timer.Wait();
    
    //not on FPGA
    //WriteReg(0x314 >> 2, 0x4000, 0x00C0); // Set AGC to hold

    extern volatile uint16_t DEBUG[];

    auto rssi_post_agc_raw = ReadReg(0x274 >> 2)[0] & 0xFF;
    SwitchToStandby();

    int16_t rssi_post_agc = (rssi_post_agc_raw & 0x80) ?
      (rssi_post_agc_raw | 0xFF00) :
      rssi_post_agc_raw;

    auto gain_data = ReadReg(0x384 >> 2);
    auto lna = (gain_data[0] >> 8) & 0xF;
    auto tia = (gain_data[0] >> 12) & 0xF;
    auto vga = (gain_data[0] >> 0) & 0xF;

    auto rssi = rssi_post_agc - RT569DigDetail::lna_gain_codes[lna] -
                RT569DigDetail::tia_gain_codes[tia] -
                RT569DigDetail::vga_gain_codes[vga];

    return rssi;
  }

  static void AlterConfigData(uint16_t, uint16_t, uint16_t) {}

private:
  void WriteReg(uint16_t addr, uint16_t MSW, uint16_t LSW) {
    spi_.Push(0xC0 | (addr >> 8), addr, 0, 0, LSW & 0xFF, LSW >> 8, MSW & 0xFF,
              MSW >> 8);
  }

  // Data is a bit weird due to endianness, it will load like so:
  //   data[0] = { (byte[0] << 8) | byte[1] }
  //   data[1] = { (byte[2] << 8) | byte[3] }
  std::array<uint16_t, 2> ReadReg(uint16_t addr) {
    std::array<uint16_t, 2> data;
    spi_.GetBuffer(data.data(), 2, 0x80 | (addr >> 8), addr, 0, 0);
    return data;
  }

  template <typename Arr>
  void WriteInitData(uint16_t address, const Arr &data) {
    for (size_t i = 0; i < data.size(); ++i) {
      WriteReg((address >> 2) + i, data[i][0], data[i][1]);
    }
  }

  void ConfigureRadio() {
    spi_.Push(0x1E, 0x01);

    if (Config::INIT_RF) {
      WriteInitData(RT569DigDetail::kRfDataBaseAddress, RT569DigDetail::kRfInitData);
    }

    if (Config::INIT_MODEM) {
      //this is the write order in their verilog tb
        for (size_t i = 0; i < RT569DigDetail::init_a_la_cart.size(); ++i) {
          WriteReg(RT569DigDetail::init_a_la_cart[i][0],
                   RT569DigDetail::init_a_la_cart[i][1],
                   RT569DigDetail::init_a_la_cart[i][2]);
      }
      WriteInitData(RT569DigDetail::kModemDataBaseAddress,
                    RT569DigDetail::kModemInitData);
    }

    if (Config::INIT_PMU) {
      WriteInitData(RT569DigDetail::kPmuDataBaseAddress,
                    RT569DigDetail::kPmuInitData);
    }

    for (size_t i = 0; i < RT569DigDetail::settings_2048_mode.size(); ++i) {
      WriteReg(RT569DigDetail::settings_2048_mode[i][0],
               RT569DigDetail::settings_2048_mode[i][1],
               RT569DigDetail::settings_2048_mode[i][2]);
    }
  }

  SPIwCS &spi_;
  BBU &bbu_;
};

template <class Cfg, class Pa, class S, class B, class Port, class Pin>
constexpr RT569DigFPGA<Cfg, Pa, S, B, Port, Pin>
MakeRT569DigFPGA(Cfg, Pa &pa, S &spi, B &bbu, Port &, Pin &) {
  return {spi, bbu};
}

#endif
