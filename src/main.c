#include "connection_controller.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "pc_comm.h"
#include "periodic_task.h"
#include <stdio.h>
#include <stdlib.h>
#include "servo.h"
#include <avr/delay.h>
#include "hc_sr04.h"

void loop() {
  char buffer[8];
    // Rotate the servo motor from 15 to 165 degrees
    for (int i = 15; i <= 165; i++) {
        // Move the servo to the current degree
        servo(i);
        // _delay_ms(30);

        //Take a measurement with the sensor
        uint16_t distance = hc_sr04_takeMeasurement();
        sprintf(buffer, "%d,%d.\n", i, distance);
        pc_comm_send_string_blocking(buffer);

        // // Print the result
        // printf("%d,%d.", i, distance);
    }

    // Repeat the previous lines from 165 to 15 degrees
    for (int i = 165; i > 15; i--) {
        // Move the servo to the current degree
        servo(i);
        // _delay_ms(30);

        // // Take a measurement with the sensor
        uint16_t distance = hc_sr04_takeMeasurement();
        sprintf(buffer, "%d,%d.\n", i, distance);
        pc_comm_send_string_blocking(buffer);

        // // Print the result
        // printf("%d,%d.", i, distance);
    }
}

int main() {
  pc_comm_init(9600, NULL);
  hc_sr04_init();

  // monitoring_system_controller_init();
  // connection_controller_init();

  // timer_init_a(monitoring_system_controller_execute, 5000);
  while (1) {
    loop();
  }

  return 0;
}

/*

int main() {
  // display_init();
  // int i = 0;
  pc_comm_init(9600, NULL);
  wifi_init();
  wifi_command_join_AP("madinnit", "12345678");
  // // WIFI_ERROR_MESSAGE_t message =
  wifi_command_create_TCP_connection("192.168.90.98", 23, NULL, NULL);

  // char buffer[100];
  char str[44];
  uint8_t humidity_integer, humidity_decimal, temperature_integer,
      temperature_decimal;

  while (1) {
    if (dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer,
                  &temperature_decimal) == DHT11_OK) {
      sprintf(str, "Humidity = %d.%d%% and the temperature = %d.%dC\n",
              humidity_integer, humidity_decimal, temperature_integer,
              temperature_decimal);
      pc_comm_send_string_blocking(&str);
      wifi_command_TCP_transmit(&str, 44);
      _delay_ms(2000);
    }

    // pc_comm_send_string_blocking("Hello World!\n");
    //  char message_str;
    //  switch (message)
    //  {
    //  case WIFI_OK:
    //      message_str = "Command successful.";
    //      break;
    //  case WIFI_FAIL:
    //      message_str = "General failure or operation not successful.";
    //      break;
    //  case WIFI_ERROR_RECEIVED_ERROR:
    //      message_str = "Received an error message from the module.";
    //      break;
    //  case WIFI_ERROR_NOT_RECEIVING:
    //      message_str = "No data received from the module.";
    //      break;
    //  case WIFI_ERROR_RECEIVING_GARBAGE:
    //      message_str = "Received unintelligible data from the module.";
    //      break;
    //  default:
    //      message_str = "Unknown error.";
    //      break;
    //  }
    //  sprintf(buffer, "message: %s\n", message_str);
    //  pc_comm_send_string_blocking(buffer);
    //  display_int(i++);
    //  wifi_command_TCP_transmit(str, 51);
  }
  return 0;
}

*/