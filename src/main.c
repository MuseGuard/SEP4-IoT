#include "connection_controller.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "periodic_task.h"
#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>
#include "includes.h"

void hello() { pc_comm_send_string_blocking("Hello, from server ;D\n"); }

int main() {
  pc_comm_init(9600, NULL);

  _delay_ms(3000);
  bool result = false;
  wifi_init();

  WIFI_ERROR_MESSAGE_t connect_to_AP =
      wifi_command_join_AP("madinnit", "12345678");

  char buffer[30];
  if (connect_to_AP == WIFI_OK) {

    pc_comm_send_string_blocking("Connected to AP!\n");
    WIFI_ERROR_MESSAGE_t connect_to_server = wifi_command_create_TCP_connection(
        "192.168.214.218", 23, hello, &buffer);
  }
  while (1)
  {
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