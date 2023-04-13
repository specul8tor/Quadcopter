//
//  RT569Radio.h
//  skaa
//
//  Created by Sam Cristall
//
//

#ifndef LIBSPINE_COMMON_RT569_RADIO_H_
#define LIBSPINE_COMMON_RT569_RADIO_H_

#include "IBBU.h"
#include "IPAControl.h"
#include "IRadio.h"
#include "ISPI.h"
#include "OutputPin.h"
#include "SoftTimer.h"
#include "WirelessConsts.h"

struct StandardRT569ConfigParams {
  static constexpr size_t INITIAL_PREAMBLE = 5;
  static constexpr uint16_t PLL_TO_TX_TIME_IN_BBU_TICKS = 216;
  static constexpr uint16_t RSSI_WAIT_TIME_IN_BBU_TICKS = 100;

  static constexpr bool INIT_RF = false;
  static constexpr bool INIT_PMU = false;
  static constexpr bool INIT_MODEM = false;
};

struct RT569ConfigWithFullInitParams {
  static constexpr size_t INITIAL_PREAMBLE = 5;
  static constexpr uint16_t PLL_TO_TX_TIME_IN_BBU_TICKS = 216;
  static constexpr uint16_t RSSI_WAIT_TIME_IN_BBU_TICKS = 100;

  static constexpr bool INIT_RF = true;
  static constexpr bool INIT_PMU = true;
  static constexpr bool INIT_MODEM = true;
};

struct RT569ConfigWithModemInitParams {
  static constexpr size_t INITIAL_PREAMBLE = 5;
  static constexpr uint16_t PLL_TO_TX_TIME_IN_BBU_TICKS = 216;
  static constexpr uint16_t RSSI_WAIT_TIME_IN_BBU_TICKS = 100;

  static constexpr bool INIT_RF = false;
  static constexpr bool INIT_PMU = false;
  static constexpr bool INIT_MODEM = true;
};

namespace RT569Detail {

static constexpr uint16_t kModemDataBaseAddress = 0x200;
static constexpr std::array<std::array<uint16_t, 2>, 60> kModemInitData = {{
    {{0x0001, 0x0200}}, {{0x0200, 0x0140}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x7F00, 0x0000}},
    {{0x0000, 0x007F}}, {{0x007F, 0x007F}}, {{0x0081, 0x0200}},
    {{0x0320, 0x3F00}}, {{0x7176, 0x4129}}, {{0x4516, 0x1000}},
    {{0x0020, 0x0320}}, {{0x0205, 0x4321}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0805, 0x02FF}}, {{0x1110, 0x0E0B}}, {{0x0000, 0x0040}},
    {{0x0258, 0x0015}}, {{0x0000, 0x0100}}, {{0x1E04, 0x0040}},
    {{0x2308, 0x0320}}, {{0x0000, 0x0200}}, {{0x000F, 0x0001}},
    {{0x0002, 0x0700}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0xFE00, 0x0102}}, {{0xFDFC, 0xFCFD}},
    {{0x0A06, 0x02FF}}, {{0x1413, 0x110E}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
}};

static constexpr uint16_t kRfDataBaseAddress = 0x300;
static constexpr std::array<std::array<uint16_t, 2>, 56> kRfInitData = {{
    {{0x000F, 0x45A0}}, {{0x0000, 0x0031}}, {{0x7000, 0x0000}},
    {{0x0010, 0x9D01}}, {{0x0000, 0x0000}}, {{0x0000, 0x0080}},
    {{0x0000, 0x1500}}, {{0x0303, 0x030D}}, {{0xF783, 0x1507}},
    {{0x0000, 0x3F0B}}, {{0x0000, 0x0BDF}}, {{0x7610, 0x4F38}},
    {{0x00FF, 0xFF3A}}, {{0x0000, 0x8DE7}}, {{0xA707, 0xFFE3}},
    {{0x2580, 0x4403}}, {{0x477B, 0xC720}}, {{0x6E43, 0x20A8}},
    {{0xF344, 0x8428}}, {{0x0080, 0x8C02}}, {{0x4002, 0xC454}},
    {{0x0000, 0x2003}}, {{0x0000, 0x0000}}, {{0x050A, 0x0F14}},
    {{0x0000, 0x1248}}, {{0xFF07, 0x020F}}, {{0x050A, 0x0F14}},
    {{0x0000, 0x1248}}, {{0x0F07, 0x0F2F}}, {{0xF0F0, 0x003F}},
    {{0x0A00, 0xF709}}, {{0x0001, 0x0000}}, {{0x0000, 0x0495}},
    {{0x2B37, 0x0D93}}, {{0x0000, 0xF06F}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x3100, 0x0F00}}, {{0x10A0, 0x4100}}, {{0x0000, 0x0020}},
    {{0x0000, 0x003B}}, {{0xEEEE, 0xEEEE}}, {{0x00EE, 0xEEED}},
    {{0x0001, 0xFFFF}}, {{0xFFFF, 0x003B}}, {{0x1F14, 0x3230}},
    {{0x0000, 0x0000}}, {{0x0017, 0x0001}}, {{0x0001, 0x0003}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0017, 0x0001}},
    {{0x0001, 0x0003}}, {{0x0000, 0x0001}},
}};

static constexpr uint16_t kPmuDataBaseAddress = 0x400;
static constexpr std::array<std::array<uint16_t, 2>, 64> kPmuInitData = {{
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0003}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x6B5F, 0xF9FE}},
    {{0x800A, 0x7D00}}, {{0x0103, 0xFFC5}}, {{0xD000, 0x0080}},
    {{0x3F3F, 0xF47F}}, {{0x0000, 0x0020}}, {{0x0000, 0x0000}},
    {{0x0004, 0x3210}}, {{0x2314, 0x0400}}, {{0x9410, 0x0100}},
    {{0x0000, 0xD0FB}}, {{0x07FF, 0xC03F}}, {{0x0000, 0x0000}},
    {{0x1000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0003}},
    {{0x0064, 0x0000}}, {{0x006E, 0x0064}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0064}},
    {{0x0000, 0x0064}}, {{0x0000, 0x0064}}, {{0x0000, 0x0064}},
    {{0x0000, 0x0073}}, {{0x00F9, 0x0000}}, {{0x00F7, 0x7777}},
    {{0x0000, 0x7777}}, {{0x6666, 0x6666}}, {{0x6666, 0x6666}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0F0F, 0x8000}},
    {{0x0F0F, 0x0F0F}}, {{0xB0FA, 0x7FFF}}, {{0xB0FA, 0x0888}},
    {{0xFF00, 0xFF00}}, {{0x020F, 0x6868}}, {{0x0000, 0x000E}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x0000, 0x0000}}, {{0x0000, 0x0000}},
    {{0x0000, 0x0000}}, {{0x003F, 0x003F}}, {{0x0000, 0x283F}},
    {{0x0000, 0x6A10}},
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

static constexpr std::array<std::array<uint16_t, 3>, 16> settings_2048_mode = {{
    {{0x304 >> 2, 0x0001, 0x00F2}}, // RF_TRX1
    {{0x340 >> 2, 0x049B, 0xC028}}, // RF_TRX2
    {{0x344 >> 2, 0x20B4, 0xC088}}, // RF_TRX3
    {{0x200 >> 2, 0x0000, 0x0400}}, // Tx_system
    {{0x204 >> 2, 0x0200, 0x0040}}, // Disable GFSK
    {{0x220 >> 2, 0x0080, 0x0400}}, // Rx_system
    {{0x224 >> 2, 0x04B0, 0x3F02}}, // Rx_packet_sync
    {{0x22C >> 2, 0x1347, 0x2000}}, // Rx_CFO_tracking
    {{0x230 >> 2, 0x0028, 0x04B0}}, // Rx_timing_tracking
    {{0x248 >> 2, 0x0000, 0x0000}}, // Rx_FIR_coef_S1_C
    {{0x24C >> 2, 0x7F00, 0x0000}}, // Rx_FIR_coef_S1_D
    {{0x258 >> 2, 0x0000, 0x0100}}, // RX_down_sample_setting
    {{0x25C >> 2, 0x2104, 0x0040}}, // RX_Rest
    {{0x264 >> 2, 0x0000, 0x0100}}, // Tx_upsampling
    {{0x448 >> 2, 0x1001, 0x0000}}, // Enable Crystal Tuning
    {{0x420 >> 2, 0xD000, 0x0029}}, // Set crystal high
}};
} // namespace RT569Detail

template <class Config, class PAControlTy, class SPIwCS, class BBUTy,
          class RadioPort, class BBUPinTy>
class RT569Radio
    : public IRadio<
          RT569Radio<Config, PAControlTy, SPIwCS, BBUTy, RadioPort, BBUPinTy>> {
public:
  using BBU = IBaseBandUnit<BBUTy>;
  using BBUPin = IOutputPin<BBUPinTy>;

  RT569Radio(SPIwCS &spi, BBU &bbu) : spi_(spi), bbu_(bbu) {

    // Configure/calibrate the radio
    ConfigureRadio();
  }

  //! Change the rf channel of the RT569
  void ChangeChannel(size_t channel) {
    assert(channel <= 49 && "RT569 channel must be <= 49");
    bbu_.PushWord(0);
    bbu_.PushWord(0);
    WriteReg(0x300 >> 2, (RT569Detail::channel_settings[channel][0] << 6) | 15,
             RT569Detail::channel_settings[channel][1]);
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

    WriteReg(0x3A8 >> 2, 0x0000, RT569Detail::power_settings[power_level]);
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
    WriteReg(0x314 >> 2, 0x4000, 0x00CF); // Start AGC
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

    WriteReg(0x314 >> 2, 0x4000, 0x00C0); // Set AGC to hold

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

    auto rssi = rssi_post_agc - RT569Detail::lna_gain_codes[lna] -
                RT569Detail::tia_gain_codes[tia] -
                RT569Detail::vga_gain_codes[vga];

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
      WriteInitData(RT569Detail::kRfDataBaseAddress, RT569Detail::kRfInitData);
    }

    if (Config::INIT_MODEM) {
      WriteInitData(RT569Detail::kModemDataBaseAddress,
                    RT569Detail::kModemInitData);
    }

    if (Config::INIT_PMU) {
      WriteInitData(RT569Detail::kPmuDataBaseAddress,
                    RT569Detail::kPmuInitData);
    }

    for (size_t i = 0; i < RT569Detail::settings_2048_mode.size(); ++i) {
      WriteReg(RT569Detail::settings_2048_mode[i][0],
               RT569Detail::settings_2048_mode[i][1],
               RT569Detail::settings_2048_mode[i][2]);
    }
  }

  SPIwCS &spi_;
  BBU &bbu_;
};

template <class Cfg, class Pa, class S, class B, class Port, class Pin>
constexpr RT569Radio<Cfg, Pa, S, B, Port, Pin>
MakeRT569Radio(Cfg, Pa &pa, S &spi, B &bbu, Port &, Pin &) {
  return {spi, bbu};
}

#endif
