#include "buttons.h"
#include "buttons_controller.h"
#include "connection_controller.h"
#include "display.h"
#include "display_controller.h"
#include "includes.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "periodic_task.h"
#include "security_system_controller.h"
#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  _delay_ms(3000);
  display_init();
  buttons_init();
  pc_comm_init(9600, NULL);

  connection_controller_init();
  monitoring_system_controller_init();

  timer_init_a(monitoring_system_controller_execute, 5000);
  security_system_control_unlock();
  while (1) {
    /* code */
  }
  return 0;
}