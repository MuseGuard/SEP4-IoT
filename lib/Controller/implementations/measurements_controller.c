#include "measurements_controller.h"
#include "dht11.h"
#include "light.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <alloca.h>

void measurements_controller_init() {
  dht11_init();
  light_init();
}

char *measurements_controller_get_temperature_and_humididty() {
  uint8_t temperature_integer, temperature_decimal, humidity_integer,
      humidity_decimal;
  char* str = alloca(20);
  if (dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer,
                &temperature_decimal) == DHT11_OK) {
    sprintf(str, "T=%d.%d/H=%d/\n", temperature_integer, temperature_decimal,
            humidity_integer);
    // _delay_ms(2000);
    return str;
  }
}
char *measurements_controller_get_lightlevels() {
    uint16_t light = light_read();
    char* str = alloca(20);
    sprintf(str, "L=%d/\n", light);
    return str;
}