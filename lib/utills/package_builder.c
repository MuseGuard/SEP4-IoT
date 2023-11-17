#include "package_builder.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

Package package_builder_build(uint8_t temperature_integer,
                              uint8_t temperature_decimal,
                              uint8_t humidity_integer, uint16_t light) {

  char str[44];
  //   dd-mm-yyyy:hh-mm-ss/T:tt.t/H:hh/L:llll
  if (temperature_integer != NULL && temperature_decimal!= NULL) {

  }

  /* sprintf(str, "Humidity = %d.%d%% and the temperature = %d.%dC\n",
          humidity_integer, humidity_decimal, temperature_integer,
          temperature_decimal); */
  Package package;
  package.data = str;
  package.size = strlen(package.data);
  // add more fields and assign values as needed
  return package;
}
