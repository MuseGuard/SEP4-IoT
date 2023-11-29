#include "buttons.h"
#include "buttons_control.h"
#include "connection_control.h"
#include "display.h"
#include "display_control.h"
#include "includes.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "periodic_task.h"
#include "security_system_control.h"
#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  display_init();
  buttons_init();

  pc_comm_init(9600, NULL);

  connection_control_init();
  monitoring_system_control_init();

  timer_init_a(monitoring_system_control_execute, 5000);
  // timer_init_b(buttons_control_listen, 100  );

  /* uint8_t pin_code[4] = {5, 6, 7, 8};
  security_system_control_change_pin_code((uint8_t *)&pin_code); */
  while (1) {
    buttons_control_listen();
    _delay_ms(100);
  }
  return 0;
}