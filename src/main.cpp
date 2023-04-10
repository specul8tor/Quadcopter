
#include "init.h"
#include "esc.h"




int main(void)
{
  init();
  wait_ms(5000);
  // xpd_puts("Start\n");
  // send_dshot(0b1010101);
  // send_dshot(0b101010101010, &motors[0]);
  while(1) {

#ifdef INIT
    blink_leds(1,1,250);
    blink_leds(0,1,500);
#endif
    // send_dshot(0b101010101010, &motors[0].motor);
    // send_dshot(0b101010101010, &motors[1]);
    // send_dshot(0b101010101010, &motors[2]);
    // send_dshot(0b101010101010, &motors[3]);
    // for(uint8_t i = 0; i < 2; i++){
    //   dshot_0(&motors[0]);
    //   dshot_1(&motors[0]);
    // }
  }

  return 0;
}
