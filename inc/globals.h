#ifndef GLOBAL_H
#define GLOBAL_H

#include <SystemClock.h>
#include <Structs.h>

#define INIT
// #define DEV
#define VERBOSE
// #define CONFIG_FILE

#ifdef DEV
const enum crystal_freq kCrysF = crys_12_288_MHz;
#else
const enum crystal_freq kCrysF = crys_24_576_MHz;
#endif
const enum sys_freq kSystemF = _49_152_MHz;

typedef struct PWM {
  GlobalPin const pwm_out;
  const uint16_t period_ms; //if >100ms, beware of overflow in duty cycle funcs
} PWM;

#define DEFAULT_IO_CFG IO_FAST_SLEW|IO_PULL_UP|IO_PULL_ENABLE|IO_DRIVE_4mA

#define LED1 { GPIO_A, io_PA, 0x1, Polar_ActiveHigh }
#define LED2 { GPIO_A, io_PA, 0x2, Polar_ActiveHigh }

#define led1_pwm   { LED1, 500 }
#define led2_pwm   { LED2, 500 }

uint8_t num_leds = 2;
static PWM all_leds[] = { led1_pwm, led2_pwm };

typedef struct MOTOR_DSHOT {
  GlobalPin motor;
  uint16_t throttle;
} MOTOR_DSHOT;

#define NUM_MOTORS 4

#define MOTOR1 { GPIO_D, io_PD4, 0x10, Polar_ActiveHigh }
#define MOTOR2 { GPIO_D, io_PD5, 0x20, Polar_ActiveHigh }
#define MOTOR3 { GPIO_D, io_PD6, 0x30, Polar_ActiveHigh }
#define MOTOR4 { GPIO_D, io_PD7, 0x40, Polar_ActiveHigh }

MOTOR_DSHOT motors[NUM_MOTORS] = { { MOTOR1, 682}, { MOTOR2, 48}, { MOTOR3, 2047}, { MOTOR4, 0} };//, MOTOR2, MOTOR3, MOTOR4};

int16_t acce_gyro_data[12];

#endif // GLOBALS.H