
#include "init.h"

int main(void)
{
  init();
  wait_ms(500);

  while (1)
  {
#ifdef INIT
    blink_leds(1, 1, 250);
    blink_leds(0, 1, 500);
#endif
  }

  return 0;
}
