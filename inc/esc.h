#ifndef ESC_H
#define ESC_H


#include "globals.h"
#include "utils.h"

static void dshot_1(GlobalPin * motor)
{
  globalPin_write(ON, motor);
  sys_clock_wait(4657);
  globalPin_write(OFF, motor);
  sys_clock_wait(1565);
}

static void dshot_0(GlobalPin * motor)
{
  globalPin_write(ON, motor);
  sys_clock_wait(2329);
  globalPin_write(OFF, motor);
  sys_clock_wait(3894);
}

static uint16_t dshot_crc(uint16_t throttle){
  uint16_t csum = 0;
  uint16_t csum_data = throttle;
  for (int i = 0; i < 3; i++) {
    csum ^= csum_data; // xor data by nibbles
    csum_data >>= 4;
  }
  csum &= 0xf;
  // append checksum
  throttle = (throttle << 4) | csum;
  return throttle;
}

static void send_dshot(uint16_t throttle, GlobalPin* motor){
  // print_int("throttle: ", throttle, XPD_Flag_Hex,1);
  uint16_t packet = dshot_crc(throttle);
  // print_int("CRC: ", packet, XPD_Flag_Hex,1);
  // print_int("size: ",sizeof(packet),XPD_Flag_UnsignedDecimal,1);
  for (uint8_t i = 0; i < 16; i++) {
    int bit = (packet >> i) & 1; // Shift the integer to the right by i bits, then AND with 1 to get the i-th bit
    // print_int("bits", bit, XPD_Flag_Hex, 1);
    if (bit){
      dshot_1(motor);
    }
    else{
      dshot_0(motor);
    }
  }
}



#endif //ESC_H