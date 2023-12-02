#include "package_builder.h"
#include "includes.h"
#include "pc_comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Static variable for Package data
static Package package;
static Package motionPackage;

Package package_builder_build_monitor(int8_t temperature_integer,
                                      uint8_t temperature_decimal,
                                      uint8_t humidity_integer,
                                      uint16_t light_levels) {
  char str[30];
  sprintf(str, "T=%d.%d/H=%d/L=%d\n", temperature_integer, temperature_decimal,
          humidity_integer, light_levels);

  strcpy(package.data, str);
  package.size = strlen(package.data);
  return package;
}

Package package_builder_build_motion_detected() {
  char str[30];
  sprintf(str, "MOTION DETECTED");

  strcpy(motionPackage.data, str);
  motionPackage.size = strlen(motionPackage.data);
  return motionPackage;
}
