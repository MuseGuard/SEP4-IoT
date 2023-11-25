#include "package_builder.h"
#include "pc_comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes.h"

// Static variable for Package data
static Package package;
static Package motionPackage;
static Package acknowledgementPackage;

Package package_builder_build_monitor(int8_t temperature_integer, uint8_t temperature_decimal, uint8_t humidity_integer, uint16_t light_levels)
{
  char str[30];
  sprintf(str, "T=%d.%d/H=%d/L=%d\n", temperature_integer, temperature_decimal, humidity_integer, light_levels);

  strcpy(package.data, str);
  package.size = strlen(package.data);
  // pc_comm_send_string_blocking(&package);
  return package;
}


Package package_builder_build_motion_detected(){
  char str[30];
  sprintf(str, "MOTION DETECTED\n");

  strcpy(motionPackage.data, str);
  motionPackage.size = strlen(motionPackage.data);
  // pc_comm_send_string_blocking(&package);
  return motionPackage;
  

}

Package package_builder_build_acknowledgement(char* message) {
  //"ACK/OK" // "ACK/ERROR"
  sprintf(acknowledgementPackage.data, "%s\n", message);
  package.size = strlen(acknowledgementPackage.data);
  return acknowledgementPackage;
}

// char str[30];
// sprintf(str, "T=%d.%d/H=%d/L=%d\n", temperature_and_humidity[0], temperature_and_humidity[1], temperature_and_humidity[2], light_levels);

// Package package;
// package.data = str;
// package->size = strlen(package->data);

// // package->data = str;
// // package->size = strlen(package->data);
// return package;