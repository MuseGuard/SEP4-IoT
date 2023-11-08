#include "display.h"
#include "pc_comm.h"
#include "wifi.h"
#include "dht11.h"
#include <util/delay.h>
#include <stdio.h>

int main()
{
    // display_init();
    // int i = 0;
    pc_comm_init(9600, NULL);
    wifi_init();
    dht11_init();
    wifi_command_join_AP("madinnit", "12345678");
    // // WIFI_ERROR_MESSAGE_t message = 
        wifi_command_create_TCP_connection("192.168.90.98", 23, NULL, NULL);

    //char buffer[100];
    char str[44];
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;

    while (1)
    {
        if (dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal) == DHT11_OK)
        {
            sprintf(str, "Humidity = %d.%d%% and the temperature = %d.%dC\n",
                    humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
            pc_comm_send_string_blocking(&str);
            wifi_command_TCP_transmit(&str, 44);
            _delay_ms(2000);
        }


        //pc_comm_send_string_blocking("Hello World!\n");
        // char message_str;
        // switch (message)
        // {
        // case WIFI_OK:
        //     message_str = "Command successful.";
        //     break;
        // case WIFI_FAIL:
        //     message_str = "General failure or operation not successful.";
        //     break;
        // case WIFI_ERROR_RECEIVED_ERROR:
        //     message_str = "Received an error message from the module.";
        //     break;
        // case WIFI_ERROR_NOT_RECEIVING:
        //     message_str = "No data received from the module.";
        //     break;
        // case WIFI_ERROR_RECEIVING_GARBAGE:
        //     message_str = "Received unintelligible data from the module.";
        //     break;
        // default:
        //     message_str = "Unknown error.";
        //     break;
        // }
        // sprintf(buffer, "message: %s\n", message_str);
        // pc_comm_send_string_blocking(buffer);
        // display_int(i++);
        // wifi_command_TCP_transmit(str, 51);

    }
    return 0;
}