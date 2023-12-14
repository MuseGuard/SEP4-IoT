#ifndef WINDOWS_TEST_APPLICATION
#include "buttons_control.h"
#include "buttons.h"
#include "includes.h"
void buttons_control_init() { buttons_init(); }

uint8_t buttons_control_listen() {
  if (buttons_1_pressed()) {
    return 1;
  } else if (buttons_2_pressed()) {
    return 2;
  } else if (buttons_3_pressed()) {
    return 3;
  } else {
    return 0;
  }
}
#endif