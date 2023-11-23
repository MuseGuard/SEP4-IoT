#include "buttons.h"
#include "buttons_controller.h"
#include "connection_controller.h"
#include "includes.h"
#include "display.h"
#include "display_controller.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "periodic_task.h"
#include "wifi.h"
#include "security_system_controller.h"
#include <stdio.h>
#include <stdlib.h>

void hello() { pc_comm_send_string_blocking("Hello, from server ;D\n"); }

int main() {
  _delay_ms(3000);
  pc_comm_init(9600, NULL);
  connection_controller_init();
  display_init();
  buttons_init();
  security_system_control_unlock();
  while (1) {
    /* code */
  }