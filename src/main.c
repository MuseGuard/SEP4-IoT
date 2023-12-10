#include "Application.h"
#include "buttons.h"
#include "display.h"
#include "includes.h"
#include "pc_comm.h"
#include "periodic_task.h"
#include "wifi.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  application_init();

  while (1) {
    application_run();
  }
  return 0;
}