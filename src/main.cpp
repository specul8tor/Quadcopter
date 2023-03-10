/* Author: Thia Wyrod
 * License: GPLv3
 */
#include <stdlib.h>
#include <IOConfig.h>
#include <Thread.h>
#include <Structs.h>
#include <XPD.h>
#include "bmi270.h"
#include "bmi2_ois.h"


int main(void)
{
  xpd_puts("Hello World\n");
  int8_t rslt;

  /* Variable to define limit to print accel data. */
  uint8_t limit = 20;

  /* Assign accel sensor to variable. */
  uint8_t sensor_list = BMI2_ACCEL;

  /* Sensor initialization configuration. */
  struct bmi2_dev bmi2_dev;

  /* Create an instance of sensor data structure. */
  struct bmi2_sens_data sensor_data = { { 0 } };

  /* Initialize the interrupt status of accel. */
  uint16_t int_status = 0;

  uint8_t indx = 0;

  bmi2_dev.intf = BMI2_SPI_INTF;
  rslt = bmi270_init(&bmi2_dev);

  xpd_puts("Chip ID: ");
  xpd_echo_int(bmi2_dev.chip_id,XPD_Flag_Hex);

  xpd_puts("SPI Check: ");
  xpd_echo_int(bmi2_dev.dummy_byte,XPD_Flag_Hex);

  // while (1) {}
  return 0;
}
