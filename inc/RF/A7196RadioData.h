#ifndef LIBSPINE_COMMON_A7196_RADIO_DATA_H_
#define LIBSPINE_COMMON_A7196_RADIO_DATA_H_

#include <array>

static constexpr uint16_t A7196_REG_DONT_CONFIG = 0xFFFF;

static constexpr uint16_t A7196_CALIBRATION_INITAL = 0x03;
static constexpr uint16_t A7196_CALIBRATION_CHANNEL = 0x1C;
static constexpr uint16_t A7196_MODE_INITIAL = 0;

static constexpr uint16_t A7196_MODECTRL_MEASURE_ADC = 0x01;
static constexpr uint16_t A7196_MODECTRL_AUTO_IF_OFFSET = 0x20;
//! note that the A7196 will only start measuring RSSI on the next StrobeRx
static constexpr uint16_t A7196_MODECTRL_MEASURE_RSSI = 0x40;

/// Notes:
///   CKO = 0x00 Normal, 0xBA ->Fsyck/64*/
///   GPIO1 = GIO1 TRXD pin
///   GPIO2 = GIO2 SDO (MISO for 4 wire SPI)
///   DATA = DATARATE = 0x17 for 12.288MHz clock
///   PLL2 = 24000.001 MHz RFbase
///   CHGX = Channel Group I and II
///   VCODEVX = VCO DEV Cal. I, II and III
///   VCOMD = VCO Mod. delay
///   PAMAX = PAsetting max power
///   RFANT = RF Analog Test

enum class A7196Reg {
  MODE,
  MODECTRL,
  CALIBRATION,
  FIFO1,
  FIFO2,
  FIFO,
  IDCODE,
  RCOSC1,
  RCOSC2,
  RCOSC3,
  CKO,
  GPIO1,
  GPIO2,
  DATARATE,
  PLL1,
  PLL2,
  PLL3,
  PLL4,
  PLL5,
  CHGROUP1,
  CHGROUP2,
  TX1,
  TX2,
  DELAY1,
  DELAY2,
  RX,
  RXGAIN1,
  RXGAIN2,
  RXGAIN3,
  RXGAIN4,
  RSSI,
  ADC,
  CODE1,
  CODE2,
  CODE3,
  IFCAL1,
  IFCAL2,
  VCOCCAL,
  VCOCAL1,
  VCOCAL2,
  VCODEVCAL1,
  VCODEVCAL2,
  DASP_REG,
  VCOMODDELAY,
  BATTERY,
  TXTEST,
  RXDEM1,
  RXDEM2,
  CPC1,
  CPC2,
  CRYSTALTEST,
  PLLTEST,
  VCOTEST,
  RFANALOG,
  IFAT,
  CHSELECT,
  ROMP_REG,
  NEW_REG_1,
  NEW_REG_2,
  NEW_REG_3,
  NEW_REG_4,
  NEW_REG_5,
  NEW_REG_6,
  NEW_REG_7,
};

// Use a template to do inline statics -- This isn't necessary after C++17
template <typename T = void> struct A7196_2Mbit_Data_Impl {
  static constexpr std::array<std::uint16_t, 64> Base = {{
      A7196_REG_DONT_CONFIG, // MODE,
      0x20,                  // MODECTRL,
      0x00,                  // CALIBRATION,
      0x3F,                  // FIFO1,
      A7196_REG_DONT_CONFIG, // FIFO2,
      A7196_REG_DONT_CONFIG, // FIFO,
      A7196_REG_DONT_CONFIG, // IDCODE,
      0x00,                  // RCOSC1,
      0x00,                  // RCOSC2,
      0x00,                  // RCOSC3,
      0x82,                  // CKO,
      0x19,                  // GPIO1,
      0x1D,                  // GPIO2,
      0x16,                  // DATARATE,
      0x50,                  // PLL1,
      0x0A,                  // PLL2,
      0xC3,                  // PLL3,
      0x50,                  // PLL4,
      0x05,                  // PLL5,
      0x3C,                  // CHGROUP1,
      0x78,                  // CHGROUP2,
      0x2E,                  // TX1,
      0x56,                  // TX2,
      0x18,                  // DELAY1,
      0x40,                  // DELAY2,
      0x70,                  // RX,
      0xFC,                  // RXGAIN1,
      0xCA,                  // RXGAIN2,
      0x9C,                  // RXGAIN3,
      0xCA,                  // RXGAIN4,
      0x00,                  // RSSI,
      0xF1,                  // ADC,
      0x00,                  // CODE1,
      0x0F,                  // CODE2,
      0x00,                  // CODE3,
      0x05,                  // IFCAL1,
      A7196_REG_DONT_CONFIG, // IFCAL2,
      0xCF,                  // VCOCCAL,
      0xD0,                  // VCOCAL1,
      0x80,                  // VCOCAL2,
      0x70,                  // VCODEVCAL1,
      0x36,                  // VCODEVCAL2,
      0x00,                  // DASP_REG,
      0xBC,                  // VCOMODDELAY,
      0x70,                  // BATTERY,
      0x7F,                  // TXTEST,
      0x07,                  // RXDEM1,
      0xC4,                  // RXDEM2,
      0xF3,                  // CPC1,
      0x33,                  // CPC2,
      0x4D,                  // CRYSTALTEST,
      0x19,                  // PLLTEST,
      0x0A,                  // VCOTEST,
      0x70,                  // RFANALOG,
      0x00,                  // IFAT,
      0x37,                  // CHSELECT,
      0x00,                  // ROMP_REG,
      0x00,                  // NEW_REG_1,
      0x00,                  // NEW_REG_2,
      0x00,                  // NEW_REG_3,
      0x00,                  // NEW_REG_4,
      0x00,                  // NEW_REG_5,
      0x00,                  // NEW_REG_6,
      0x00                   // NEW_REG_7,
  }};
  static constexpr std::array<std::uint16_t, 12> Addr2A = {
      {0x39, 0x09, 0xF0, 0x80, 0x80, 0x08, 0x82, 0xC0, 0x00, 0x3C, 0xE8, 0x00}};
  static constexpr std::array<std::uint16_t, 9> Addr38 = {
      {0x00, 0x00, 0x30, 0xB4, 0x20, 0x90, 0x00, 0x00, 0x00}};
};

template <typename T = void> struct A7196_4Mbit_Data_Impl {
  static constexpr std::array<std::uint16_t, 64> Base = {{
      A7196_REG_DONT_CONFIG, // MODE,
      0x20,                  // MODECTRL,
      0x00,                  // CALIBRATION,
      0x3F,                  // FIFO1,
      A7196_REG_DONT_CONFIG, // FIFO2,
      A7196_REG_DONT_CONFIG, // FIFO,
      A7196_REG_DONT_CONFIG, // IDCODE,
      0x00,                  // RCOSC1,
      0x00,                  // RCOSC2,
      0x00,                  // RCOSC3,
      0xEA,                  // CKO,
      0x19,                  // GPIO1,
      0x1D,                  // GPIO2,
      0x2E,                  // DATARATE,
      0x50,                  // PLL1,
      0x16,                  // PLL2,
      0x61,                  // PLL3,
      0xa8,                  // PLL4,
      0x00,                  // PLL5,
      0x3B,                  // CHGROUP1,
      0x75,                  // CHGROUP2,
      0x2E,                  // TX1,
      0x2A,                  // TX2,
      0x19,                  // DELAY1,
      0x40,                  // DELAY2,
      0x70,                  // RX,
      0xFC,                  // RXGAIN1,
      0xCA,                  // RXGAIN2,
      0xBC,                  // RXGAIN3,
      0xCA,                  // RXGAIN4,
      0x00,                  // RSSI,
      0xF1,                  // ADC,
      0x00,                  // CODE1,
      0x0F,                  // CODE2,
      0x00,                  // CODE3,
      0x05,                  // IFCAL1,
      A7196_REG_DONT_CONFIG, // IFCAL2,
      0xCF,                  // VCOCCAL,
      0xD0,                  // VCOCAL1,
      0x80,                  // VCOCAL2,
      0xF0,                  // VCODEVCAL1,
      0x36,                  // VCODEVCAL2,
      0x00,                  // DASP_REG,
      0xBC,                  // VCOMODDELAY,
      0x70,                  // BATTERY,
      0x7F,                  // TXTEST,
      0x07,                  // RXDEM1,
      0xC4,                  // RXDEM2,
      0x00,                  // CPC1,
      0x00,                  // CPC2,
      0x4D,                  // CRYSTALTEST,
      0x19,                  // PLLTEST,
      0x0A,                  // VCOTEST,
      0x70,                  // RFANALOG,
      0x00,                  // IFAT,
      0x77,                  // CHSELECT,
      0x00,                  // ROMP_REG,
      0x00,                  // NEW_REG_1,
      0x00,                  // NEW_REG_2,
      0x00,                  // NEW_REG_3,
      0x00,                  // NEW_REG_4,
      0x00,                  // NEW_REG_5,
      0x00,                  // NEW_REG_6,
      0x00                   // NEW_REG_7,
  }};
  static constexpr std::array<std::uint16_t, 12> Addr2A = {
      {0x39, 0x09, 0xF0, 0x80, 0x80, 0x08, 0x82, 0xC0, 0x00, 0x3C, 0xE8, 0x00}};
  static constexpr std::array<std::uint16_t, 9> Addr38 = {
      {0x00, 0x00, 0x30, 0xB4, 0x20, 0xB0, 0x00, 0x00, 0x00}};
};

template <typename T>
constexpr std::array<std::uint16_t, 64> A7196_2Mbit_Data_Impl<T>::Base;
template <typename T>
constexpr std::array<std::uint16_t, 12> A7196_2Mbit_Data_Impl<T>::Addr2A;
template <typename T>
constexpr std::array<std::uint16_t, 9> A7196_2Mbit_Data_Impl<T>::Addr38;
template <typename T>
constexpr std::array<std::uint16_t, 64> A7196_4Mbit_Data_Impl<T>::Base;
template <typename T>
constexpr std::array<std::uint16_t, 12> A7196_4Mbit_Data_Impl<T>::Addr2A;
template <typename T>
constexpr std::array<std::uint16_t, 9> A7196_4Mbit_Data_Impl<T>::Addr38;

using A7196_2Mbit_Data = A7196_2Mbit_Data_Impl<void>;
using A7196_4Mbit_Data = A7196_4Mbit_Data_Impl<void>;

#endif
