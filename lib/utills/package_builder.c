#include "package_builder.h"
#include "pc_comm.h"
#include <Time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Package package; // Static variable for Package data

Package package_builder_build(int8_t temperature_integer, uint8_t temperature_decimal, uint8_t humidity_integer, uint16_t light_levels)
{
  char str[30];
  sprintf(str, "T=%d.%d/H=%d/L=%d\n", temperature_integer, temperature_decimal, humidity_integer, light_levels);

  strcpy(package.data, str);
  // pc_comm_send_string_blocking(&package);
  return package;
}

// char str[30];
// sprintf(str, "T=%d.%d/H=%d/L=%d\n", temperature_and_humidity[0], temperature_and_humidity[1], temperature_and_humidity[2], light_levels);

// Package package;
// package.data = str;
// package->size = strlen(package->data);

// // package->data = str;
// // package->size = strlen(package->data);
// return package;