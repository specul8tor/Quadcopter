
#include <XPD.h>
#include <I2C.h>

SXC_INLINE uint16_t i2c_read_value(uint16_t dest_addr, uint16_t dest_reg, uint8_t nack, I2C_Pin_Pair const* pins){
  i2c_write_to_addr(dest_addr, dest_reg, pins);
  i2c_write_start(pins);
  i2c_write_byte(dest_addr+1, pins);
  uint16_t value = i2c_read_byte(nack,pins);
  i2c_write_stop(pins);
  return value;
} 

SXC_INLINE void print_int(const char *input, int number, enum echo_flag type, uint8_t newline){
  xpd_puts(input);
  xpd_echo_int(number,type);
  if (newline == 1){
    xpd_puts("\n");
  }
}