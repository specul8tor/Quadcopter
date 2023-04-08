#ifndef UTILS_H
#define UTILS_H

#include <XPD.h>
#include <I2C.h>

SXC_INLINE uint16_t i2c_read_value(uint16_t dest_addr, uint16_t dest_reg, uint8_t nack, I2C_Pin_Pair const* pins){
  i2c_write_to_addr(dest_addr, dest_reg, pins);
  i2c_write_start(pins);
  i2c_write_byte(dest_addr+1, pins);
  int16_t value = i2c_read_byte(nack,pins);
  i2c_write_stop(pins);
  return value;
} 

SXC_INLINE uint16_t i2c_read_from_buffer(uint16_t dest_addr, uint16_t dest_reg, uint8_t num_bytes, int16_t * const buffer, I2C_Pin_Pair const* pins){
  i2c_write_to_addr(dest_addr, dest_reg, pins);
  i2c_write_start(pins);
  i2c_write_byte(dest_addr+1, pins);
  uint8_t nack = 0;
  for(uint8_t i = 0; i < num_bytes; i++){
    if (i == num_bytes - 1){
        nack = 1;
    }
    *(buffer+i) = i2c_read_byte(nack,pins);
  }
  i2c_write_stop(pins);
  return 1;
} 

SXC_INLINE void print_int(const char *input, int number, enum echo_flag type, uint8_t newline){
  xpd_puts(input);
  xpd_echo_int(number,type);
  if (newline == 1){
    xpd_puts("\n");
  }
}

static inline void wait_ms(size_t ms)
{
  for (size_t i = 0; i < ms; ++i) {
    for (size_t i = 0; i < kNumSlicesPerMs; ++i)
      sys_clock_wait(sys_clock_ticks_per_ms_slice(kSystemF));
  }
}

static inline void blink_leds(uint8_t led, uint8_t num_blinks, size_t delay_ms){

    for (uint8_t i = 0; i <= 3; i++){
      globalPin_write(ON, &all_leds[led].pwm_out);
      wait_ms(delay_ms);
      globalPin_write(OFF, &all_leds[led].pwm_out);
      wait_ms(delay_ms);
    }
}



#endif // UTILS_H