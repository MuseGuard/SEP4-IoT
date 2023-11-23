#include "buttons.h"
#include "buttons_controller.h"
#include "connection_controller.h"
#include "display.h"
#include "display_controller.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "periodic_task.h"
#include "security_system_controller.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  pc_comm_init(9600, NULL);
  display_init();
  buttons_init();
  security_system_control_unlock();
  
  while (1) {
    /* code */
  }

  // monitoring_system_controller_init();
  // connection_controller_init();

  return 0;
}