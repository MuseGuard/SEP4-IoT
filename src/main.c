#include "buttons.h"
#include "buttons_controller.h"
#include "connection_controller.h"
#include "display.h"
#include "display_controller.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "periodic_task.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  pc_comm_init(9600, NULL);
  display_init();
  buttons_init();
  // monitoring_system_controller_init();
  // connection_controller_init();
  buttons_control_pin_code_input();
  // timer_init_a(monitoring_system_controller_execute, 5000);
  while (1) {
    // display_controller_write_word("Hello World");
  }

  return 0;
}