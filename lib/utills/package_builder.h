#pragma once

#include <stdint.h>

typedef struct {
  char *data;
  uint8_t size;
} Package;

Package package_builder_build(uint8_t *temperature_and_humidity,
                              uint16_t light_levels);