#include <XPD.h>
#include <Thread.h>
#include "bmi270.h"
#include "globals.h"

static inline void init_clock(void)
{
  sys_clock_init(kCrysF, kSystemF);
}

static void led_init()
{
    for (size_t i = 0; i < num_leds; i++){
      io_set_config(DEFAULT_IO_CFG, all_leds[i].pwm_out.io_port);
      globalPin_set_dir(PinDir_Output, &all_leds[i].pwm_out);
    }
    return;
}

static void* IMU_thread(void *ptr)
{
    get_IMU();
    return NULL;
}

static void init(){

    xpd_puts("\nHello World\n");
    wait_ms(20);
    init_clock();
    led_init();
    bmi270_init();

    globalPin_write(ON, &all_leds[0].pwm_out);
    blink_leds(1, 4, 500);
    globalPin_write(ON, &all_leds[1].pwm_out);


    thread_setup(IMU_thread, NULL, 1);
    thread_run(1);

}