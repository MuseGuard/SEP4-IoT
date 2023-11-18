#include "package_builder.h"
#include "pc_comm.h"
#include <Time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Package package_builder_build(uint8_t *temperature_and_humidity,
                              uint16_t light_levels) {

  char str[30];
  sprintf(str, "T=%d.%d/H=%d/L=%d\n", temperature_and_humidity[0], temperature_and_humidity[1], temperature_and_humidity[2], light_levels);

  Package package;
  package.data = str;
  package.size = strlen(package.data);
  return package;
}
