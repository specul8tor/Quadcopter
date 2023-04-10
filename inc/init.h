#include <XPD.h>
#include <Thread.h>
#include "globals.h"
#include "utils.h"
#include "esc.h"

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

static void motor_init()
{
    for (size_t i = 0; i < NUM_MOTORS; i++){
      io_set_config(DEFAULT_IO_CFG, motors[i].motor.io_port);
      globalPin_set_dir(PinDir_Output, &motors[i].motor);
    }
    return;
}

static void* motor_thread(void* ptr){
  MOTOR_DSHOT * motor = (MOTOR_DSHOT *)ptr;
  while(1){
    send_dshot(motor->throttle, &motor->motor);
  }
}

static void init(){

    xpd_puts("\nHello World\n");
    wait_ms(20);
    init_clock();
    led_init();
    motor_init();

    globalPin_write(ON, &all_leds[0].pwm_out);
    blink_leds(1, 4, 500);
    globalPin_write(ON, &all_leds[1].pwm_out);
    
    for(size_t i = 0; i < NUM_MOTORS; i++){
      thread_setup(motor_thread, &motors[i],1+i);
      thread_run(1+i);
    }

}