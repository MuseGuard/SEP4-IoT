#include "includes.h"
#include "connection_control.h"
#include "radar_control.h"
#include "hc_sr04.h"
#include "package_builder.h"
#include "servo.h"
#include <stdint.h>
#include <stdio.h>

static uint8_t angle;
static uint16_t distance; // Static variables for Radar Data

void radar_control_init() // Initializes Radar
{
    hc_sr04_init();
    angle = 15;
    // servo(15);
}
void radar_control_measurement(uint8_t angle)
{
        char buffer[8];
        _delay_ms(100);
        servo(angle);

        //Take a measurement with the sensor
        distance = hc_sr04_takeMeasurement();
        sprintf(buffer, "%d,%d\n", angle, distance);
        connection_control_send_message(buffer);
        pc_comm_send_string_blocking(buffer);
}

void radar_control_start()
{
    while (angle > 15)
    {
        radar_control_measurement(angle);
        angle = angle - 5;
    }
    while (angle < 165)
    {
        radar_control_measurement(angle);
        angle = angle + 5;
    }
}
