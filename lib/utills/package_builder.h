#pragma once

#include <stdint.h>

typedef struct {
  char *data;
  uint8_t size;
} Package;

Package package_builder_build(char *temperature_and_humidity,
                              char *light_levels);