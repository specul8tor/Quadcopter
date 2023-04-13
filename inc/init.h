#include <XPD.h>
#include <Thread.h>
#include "bmi270.h"
#include "globals.h"
#include "utils.h"
#include "esc.h"
#ifdef KEYBOARD
#include "keys.h"
#else
#include "recieve.h"
#endif

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

#ifdef KEYBOARD
static void* key_thread(void* ptr){
  key_cmd();
  return NULL;
}
#else
static void* recieve_thread(void* ptr){
  recieve();
}
#endif

static void* IMU_thread(void *ptr)
{
    get_IMU();
    return NULL;
}

static void init(){

    xpd_puts("\nHello Wrld\n\n");
    wait_ms(5000);
    init_clock();
    led_init();
    motor_init();
    bmi270_init();

    globalPin_write(ON, &all_leds[0].pwm_out);
    blink_leds(1, 4, 500);
    globalPin_write(ON, &all_leds[1].pwm_out);
    
    thread_setup(IMU_thread, NULL, 1);
    thread_run(1);
    
    for(size_t i = 0; i < NUM_MOTORS; i++){
      thread_setup(motor_thread, &motors[i],2+i);
      thread_run(2+i);
    }

#ifdef KEYBOARD
    thread_setup(key_thread, NULL, 6);
    thread_run(6);
#else
    thread_setup(recieve_thread, NULL, 7);
    thread_run(7);
#endif

}