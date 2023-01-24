/* Author: Thia Wyrod
 * License: GPLv3
 */
#ifndef _BUF_BUTTON_H_
#define _BUF_BUTTON_H_
#include <Structs.h>

typedef struct BufferedButton {
  GlobalPin const gl_pin;
  uint16_t const threshold_cycles;
  uint16_t cycles_active;
} BufferedButton;

/* Returns 1 if the buffered button pin has been held active for exactly
 * bb->curr_cycles consecutive calls to this function, 0 else. Resets the
 * BufferedButton's cycles_active if button is inactive, else increments
 * them, up to threshold_cycles+1.
 */
static inline uint16_t read_buf_button(BufferedButton* bb)
{
  if (globalPin_read(&bb->gl_pin) == ON) {
    if (bb->cycles_active <= bb->threshold_cycles)
      ++bb->cycles_active;
  }
  else
    bb->cycles_active = 0;
  return (bb->cycles_active == bb->threshold_cycles);
}

#endif //_BUF_BUTTON_H_
