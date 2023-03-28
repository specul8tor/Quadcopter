/* Author: Thia Wyrod
 * License: GPLv3
 */
#include <stdlib.h>
#include <IOConfig.h>
#include <Thread.h>
#include <Structs.h>
#include <XPD.h>

#include "time_funcs.h"
#include "buffered_button.h"
#include "struct_aliases.h"

static const uint16_t DIVISOR_MAX = 128; //Instead of percentage, use base-2
static const uint16_t HIGH_DUTY_CYCLE = DIVISOR_MAX;
static const uint16_t LOW_DUTY_CYCLE = DIVISOR_MAX/32;

enum DutyCycleSelect {
  DutyHigh,
  DutyLow
};

typedef struct PWM_LED {
  GlobalPin const button;
  GlobalPin const pwm_out;
  const uint16_t period_ms; //if >100ms, beware of overflow in duty cycle funcs
} PWM_LED;
#define red_pwm { SW0, LED5, 100 }
#define green_pwm { SW1, LED6, 100 }
#define blue_pwm { SW2, LED3, 100 }
#define motor {SW1, LED1, 100}

static PWM_LED all_leds[] = { red_pwm, green_pwm, blue_pwm, motor };
static const uint16_t kNumPWMLEDs = sizeof(all_leds)/sizeof(*all_leds);
static const uint16_t kPWMThrdOffset = 1;

static void pwm_drive_cycle(enum DutyCycleSelect duty, PWM_LED const * pwm_led)
{
  uint16_t on_ms;
  if (duty == DutyHigh)
    on_ms = HIGH_DUTY_CYCLE*(pwm_led->period_ms)/DIVISOR_MAX;
  else if (duty == DutyLow)
    on_ms = LOW_DUTY_CYCLE*(pwm_led->period_ms)/DIVISOR_MAX;
  else
    on_ms = 0;
  globalPin_write(ON, &pwm_led->pwm_out);
  wait_ms(250);
  globalPin_write(OFF, &pwm_led->pwm_out);
  // wait_ms(pwm_led->period_ms - on_ms);
  wait_ms(250);
  // globalPin_write(ON, &pwm_led->pwm_out);
  // wait_ms(10);
  // globalPin_write(ON, &pwm_led->pwm_out);
  // wait_ms(19);
  // globalPin_write(OFF, &pwm_led->pwm_out);
  // wait_ms(1);

}

static void* pwm_drive_thread(void* ptr)
{
  PWM_LED const * const pwm_led = (PWM_LED const * const)ptr;
  enum DutyCycleSelect duty_cycle;
  while(1) {
    if (globalPin_read(&pwm_led->button) == ON)
      duty_cycle = DutyHigh;
    else
      duty_cycle = DutyLow;
    pwm_drive_cycle(duty_cycle, pwm_led);
  }
  return NULL;
}

int main(void)
{
  xpd_puts("Hello Max\n");
  init_clock();
  for (size_t i = 0; i < kNumPWMLEDs; ++i) {
    io_set_config(DEFAULT_BUTTON_CFG, all_leds[i].button.io_port);
    io_set_config(DEFAULT_LED_CFG, all_leds[i].pwm_out.io_port);
    globalPin_set_dir(PinDir_Input, &all_leds[i].button);
    globalPin_set_dir(PinDir_Output, &all_leds[i].pwm_out);
    thread_setup(pwm_drive_thread, &all_leds[i], kPWMThrdOffset+i);
    thread_run(kPWMThrdOffset+i);
  }
  while (1) {}
  return 0;
}
