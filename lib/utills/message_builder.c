
#include "message_builder.h"
#include "includes.h"
#include "pc_comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WINDOWS_TEST
char *message_builder_build_monitor(int8_t temperature_integer,
                                    uint8_t temperature_decimal,
                                    uint8_t humidity_integer,
                                    uint16_t light_levels) {
  char *str = malloc(30);
  sprintf(str, "T=%d.%d/H=%d/L=%d\n", temperature_integer, temperature_decimal,
          humidity_integer, light_levels);

  return str;
}
#endif