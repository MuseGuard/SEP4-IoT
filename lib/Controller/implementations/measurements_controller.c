#include "measurements_controller.h"
#include "dht11.h"
#include "light.h"
#include <alloca.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void measurements_controller_init() {
  dht11_init();
  light_init();
}

uint8_t *measurements_controller_get_temperature_and_humidity() {
  uint8_t temperature_integer, temperature_decimal, humidity_integer,
      humidity_decimal;
  uint8_t *measurements = malloc(3 * sizeof(uint8_t));
  if (dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer,
                &temperature_decimal) == DHT11_OK) {
    measurements[0] = temperature_integer;
    measurements[1] = temperature_decimal;
    measurements[2] = humidity_integer;
  }
    return measurements;
}
uint16_t measurements_controller_get_lightlevels() { return light_read(); }