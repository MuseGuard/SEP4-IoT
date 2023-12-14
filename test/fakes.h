#include "fff.h"
#include "buttons.h"
#include "buzzer.h"
#include "dht11.h"
#include "display.h"
#include "light.h"   
#include "pc_comm.h"
#include "periodic_task.h"
#include "pir.h"
#include "wifi.h"
#include "message_builder.h"
#include "periodic_task.h"


DEFINE_FFF_GLOBALS;
/**
 * @brief Fake all the necesarry functions for the tests
 * 
 */


//BUZZER
FAKE_VOID_FUNC(buzzer_beep);

//BUTTONS
FAKE_VOID_FUNC(buttons_init);
FAKE_VALUE_FUNC(uint8_t , buttons_1_pressed);
FAKE_VALUE_FUNC(uint8_t , buttons_2_pressed);
FAKE_VALUE_FUNC(uint8_t , buttons_3_pressed);

//DHT11
FAKE_VOID_FUNC(dht11_init);
FAKE_VALUE_FUNC(DHT11_ERROR_MESSAGE_t, dht11_get, uint8_t*, uint8_t*, uint8_t*, uint8_t*);

//DISPLAY
FAKE_VOID_FUNC(display_init);
FAKE_VOID_FUNC(display_setValues, uint8_t, uint8_t, uint8_t, uint8_t);
FAKE_VOID_FUNC(display_int, int16_t);


//LIGHT
FAKE_VOID_FUNC(light_init);
FAKE_VALUE_FUNC(uint16_t, light_read);

//PC_COMM
FAKE_VOID_FUNC(pc_comm_init, uint32_t, pc_comm_callback_t);
FAKE_VOID_FUNC(pc_comm_send_string_blocking, char*);

//PERIODIC_TASK

//PIR
FAKE_VOID_FUNC(pir_init, pir_callback_t);

//WIFI
FAKE_VOID_FUNC(wifi_init);
FAKE_VALUE_FUNC(WIFI_ERROR_MESSAGE_t, wifi_command_join_AP, char*, char*);
FAKE_VALUE_FUNC(WIFI_ERROR_MESSAGE_t, wifi_command_create_TCP_connection, char*, uint16_t, WIFI_TCP_Callback_t, char*);
FAKE_VALUE_FUNC(WIFI_ERROR_MESSAGE_t, wifi_command_TCP_transmit, uint8_t*, uint16_t);

//AVR
FAKE_VOID_FUNC(cli);
FAKE_VOID_FUNC(sei);
FAKE_VOID_FUNC(_delay_ms, int);

//Message Builder
FAKE_VALUE_FUNC(char*, message_builder_build_monitor, int8_t, uint8_t, uint8_t, uint16_t);


//Periodic Task