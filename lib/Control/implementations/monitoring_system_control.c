#ifndef WINDOWS_TEST_APPLICATION
#include "monitoring_system_control.h"
#include "dht11.h"
#include "includes.h"
#include "light.h"
#include "message_builder.h"

static uint8_t temperature_integer, temperature_decimal, humidity_integer,
    humidity_decimal;         // Static variables for Temperature Data
static uint16_t light_levels; // Static variable for Light Data
static char message[30];

void monitoring_system_control_init() // Initializes all sensors
{
  dht11_init();
  light_init();
}

char *monitoring_system_control_execute() // Getting data from all sensors and
                                          // transmitimg over wifi
{
  DHT11_ERROR_MESSAGE_t measurement_result =
      dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer,
                &temperature_decimal);
  if (measurement_result != DHT11_OK) {
    strcpy(message, "Error reading DHT11 sensor!");
  } else {
    light_levels = light_read();

    // Give the package a new value

    char *result =
        message_builder_build_monitor(temperature_integer, temperature_decimal,humidity_integer, light_levels);
    strcpy(message, result);
    free(result);
  }
  return message;
}
#endif