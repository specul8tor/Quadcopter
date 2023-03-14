
#include <IOConfig.h>
#include <Structs.h>
#include <SystemClock.h>
#include <XPD.h>
#include <Thread.h>

#define DEFAULT_LED_CFG IO_FAST_SLEW|IO_PULL_UP|IO_PULL_ENABLE|IO_DRIVE_4mA

#define LED1 { GPIO_C, io_PC0, 0x1, Polar_ActiveLow }
#define LED2 { GPIO_C, io_PC1, 0x2, Polar_ActiveLow }
#define LED3 { GPIO_D, io_PD4, 0x10, Polar_ActiveLow }
#define LED4 { GPIO_D, io_PD5, 0x20, Polar_ActiveLow }

#define led1_pwm   { LED1, 500 }
#define led2_pwm   { LED2, 500 }

typedef struct PWM_LED {
  GlobalPin const pwm_out;
  const uint16_t period_ms; //if >100ms, beware of overflow in duty cycle funcs
} PWM_LED;

const enum crystal_freq kCrysF = crys_12_288_MHz;
const enum sys_freq kSystemF = _49_152_MHz;

static inline void init_clock(void)
{
  sys_clock_init(kCrysF, kSystemF);
}

static inline void wait_ms(size_t ms)
{
  for (size_t i = 0; i < ms; ++i) {
    for (size_t i = 0; i < kNumSlicesPerMs; ++i)
      sys_clock_wait(sys_clock_ticks_per_ms_slice(kSystemF));
  }
}

static PWM_LED all_leds[] = { led1_pwm, led2_pwm };
static const uint16_t num_leds = sizeof(all_leds)/sizeof(*all_leds) + 4;

static void* pwm_led_drive_thread(void *ptr)
{
    PWM_LED const * const pwm_led = (PWM_LED const * const)ptr;
    while(1){
        uint16_t on_ms = pwm_led->period_ms;
        globalPin_write(ON, &pwm_led->pwm_out);
        wait_ms(on_ms/2);
        globalPin_write(OFF, &pwm_led->pwm_out);
        wait_ms(on_ms/2);
    }
    return NULL;
}

static void init(){
    xpd_puts("Hello World\n");
    init_clock();
    for (size_t i = 0; i < num_leds; ++i) {
        io_set_config(DEFAULT_LED_CFG, all_leds[i].pwm_out.io_port);
        globalPin_set_dir(PinDir_Output, &all_leds[i].pwm_out);
        thread_setup(pwm_led_drive_thread, &all_leds[i], 1+i);
        thread_run(1+i);
    }
}