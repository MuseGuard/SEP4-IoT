#include "connection_controller.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "periodic_task.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  pc_comm_init(9600, NULL);
  monitoring_system_controller_init();
  connection_controller_init();

  timer_init_a(monitoring_system_controller_execute, 5000);
  while (1) {
  }

  return 0;
}