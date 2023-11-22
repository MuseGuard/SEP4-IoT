#pragma once

#include <stdint.h>

typedef struct {
  char data[30];
  uint8_t size;
} Package;

Package package_builder_build(int8_t temperature_integer, uint8_t temperature_decimal, uint8_t humidity_integer, uint16_t light_levels);