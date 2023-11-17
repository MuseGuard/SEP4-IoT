#include "package_builder.h"
#include "pc_comm.h"
#include <Time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Package package_builder_build(char *temperature_and_humidity,
                              char *light_levels) {

  char str[44];
  sprintf(str, "%s/%s\n\n", temperature_and_humidity, light_levels);

  //   dd-mm-yyyy:hh-mm-ss/T:tt.t/H:hh/L:llll

  /* sprintf(str, "Humidity = %d.%d%% and the temperature = %d.%dC\n",
          humidity_integer, humidity_decimal, temperature_integer,
          temperature_decimal); */
  Package package;
  package.data = str;
  package.size = strlen(package.data);
  // add more fields and assign values as needed
  return package;
}
