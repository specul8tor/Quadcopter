
#include <XPD.h>
#include "init.h"
#include "bmi270.h"

int main(void)
{
  init();
  bmi270_init();
}
