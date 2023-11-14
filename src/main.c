#include "buttons.h"
#include "buttons_control.h"
#include "display.h"
#include "display_control.h"
#include "pc_comm.h"
#include "security_system_control.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  display_init();
  buttons_init();
  pc_comm_init(9600, NULL);
  security_system_control_unlock();
  pc_comm_send_string_blocking("Hello world\n");
  /* code */
  return 0;
}
