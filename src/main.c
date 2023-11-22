#include "connection_controller.h"
#include "includes.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "periodic_task.h"
#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>

void hello() { pc_comm_send_string_blocking("Hello, from server ;D\n"); }

int main() {
  _delay_ms(3000);
  pc_comm_init(9600, NULL);
  connection_controller_init();
  while (1) {
    /* code */
  }

  /* code */
  return 0;
}

/* int main() {
  pc_comm_init(9600, NULL);
  monitoring_system_controller_init();
  connection_controller_init();

  timer_init_a(monitoring_system_controller_execute, 5000);
  while (1) {
  }

  return 0;
} */