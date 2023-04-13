#ifndef LIBSPINE_COMMON_GINSENG_RADIO_DATA_H_
#define LIBSPINE_COMMON_GINSENG_RADIO_DATA_H_

#include <array>

static constexpr uint16_t A7125_REG_DONT_CONFIG = 0xFFFF;

static constexpr uint16_t A7125_CALIBRATION_INITAL = 0x03;
static constexpr uint16_t A7125_CALIBRATION_CHANNEL = 0x1C;
static constexpr uint16_t A7125_MODE_INITIAL = 0;

static constexpr uint16_t A7125_MODECTRL_MEASURE_ADC = 0x01;
static constexpr uint16_t A7125_MODECTRL_AUTO_IF_OFFSET = 0x20;
//! note that the A7125 will only start measuring RSSI on the next StrobeRx
static constexpr uint16_t A7125_MODECTRL_MEASURE_RSSI = 0x40;

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

enum class A7125Reg {
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
  VCODEVCAL3,
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
  VRB,
};
static constexpr std::array<uint16_t, 57> A7125Config = {{
  A7125_REG_DONT_CONFIG, // MODE,
  0x20,                  // MODECTRL,
  0x00,                  // CALIBRATION,
  0x3F,                  // FIFO1,
  A7125_REG_DONT_CONFIG, // FIFO2,
  A7125_REG_DONT_CONFIG, // FIFO,
  A7125_REG_DONT_CONFIG, // IDCODE,
  0x00,                  // RCOSC1,
  0x00,                  // RCOSC2,
  0x00,                  // RCOSC3,
  0x82,                  // CKO,
  0x1D,                  // GPIO1,
  0x19,                  // GPIO2,
  0x16,                  // DATARATE,
  0x50,                  // PLL1,
  0x0A,                  // PLL2,
  0xC3,                  // PLL3,
  0x50,                  // PLL4,
  0x05,                  // PLL5,
  0x3C,                  // CHGROUP1,
  0x78,                  // CHGROUP2,
  0xD7,                  // TX1,
  0x55,                  // TX2,
  0x10,                  // DELAY1,
  0x41,                  // DELAY2,
  0x62,                  // RX,
  0xA0,                  // RXGAIN1,
  0x00,                  // RXGAIN2,
  0x00,                  // RXGAIN3,
  0x42,                  // RXGAIN4,
  0x00,                  // RSSI,
  0xEA,                  // ADC,
  0x07,                  // CODE1,
  0x57,                  // CODE2,
  0x2A,                  // CODE3,
  0x06,                  // IFCAL1,
  A7125_REG_DONT_CONFIG, // IFCAL2,
  0x05,                  // VCOCCAL,
  0x44,                  // VCOCAL1,
  0x80,                  // VCOCAL2,
  0x30,                  // VCODEVCAL1,
  0x20,                  // VCODEVCAL2,
  0x80,                  // VCODEVCAL3,
  0x00,                  // VCOMODDELAY,
  0x7A,                  // BATTERY,
  0x27,                  // TXTEST,
  0x07,                  // RXDEM1,
  0x80,                  // RXDEM2,
  0xF1,                  // CPC1,
  0x11,                  // CPC2,
  0x05,                  // CRYSTALTEST,
  0x45,                  // PLLTEST,
  0x18,                  // VCOTEST,
  0x10,                  // RFANALOG,
  0xFF,                  // IFAT,
  0x37,                  // CHSELECT,
  0xFF                   // VRB,
  }};

#endif
