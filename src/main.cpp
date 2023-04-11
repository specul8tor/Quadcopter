
#include "init.h"

int main(void)
{
  init();
  while(1) {
    blink_leds(0,1,68);
    blink_leds(1,1,125);
    blink_leds(2,1,250);
    blink_leds(3,1,500);
  }
  return 0;
}
