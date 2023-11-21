#include "../fff.h"
#include "unity.h"
#include "wifi.h"

#include "uart.h"
#include <stdio.h>

#define TEST_WIFI_WIN

DEFINE_FFF_GLOBALS
FAKE_VOID_FUNC(sei);
FAKE_VOID_FUNC(cli);
FAKE_VOID_FUNC(_delay_ms, int);

FAKE_VOID_FUNC(uart_send_string_blocking, USART_t, char *);
FAKE_VOID_FUNC(uart_init, USART_t, uint32_t, UART_Callback_t);
FAKE_VOID_FUNC(uart_send_array_blocking, USART_t, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(UART_Callback_t, uart_get_rx_callback, USART_t);
uint8_t TEST_BUFFER[128];
void TCP_Received_callback_func();
FAKE_VOID_FUNC(TCP_Received_callback_func);

FAKE_VOID_FUNC(uart_send_array_nonBlocking, USART_t, uint8_t *, uint16_t);

void setUp(void)
{
  wifi_init();
  RESET_FAKE(uart_init);
  RESET_FAKE(TCP_Received_callback_func);
}
void tearDown(void) {}

void fake_wifiModule_send(char *cArray, int length)
{
  wifi_command_AT();
  UART_Callback_t callback = uart_init_fake.arg2_history[0];
  for (int i = 0; i < length; i++)
  {
    callback(cArray[i]);
  }
}
void test_wifi_default_callback_func_is_null()
{
  TEST_ASSERT_EQUAL(NULL, uart_init_fake.arg2_val);
}

void test_wifi_command_AT_sends_correct_stuff_to_uart()
{
  wifi_init();
  uart_get_rx_callback_fake.return_val = uart_init_fake.arg2_val;

  WIFI_ERROR_MESSAGE_t error = wifi_command_AT();

  TEST_ASSERT_EQUAL(USART_3, uart_send_string_blocking_fake.arg0_val);

  TEST_ASSERT_EQUAL(1, uart_send_string_blocking_fake.call_count);
  TEST_ASSERT_EQUAL_STRING("AT\r\n", uart_send_string_blocking_fake.arg1_val);
}

void test_wifi_command_AT_error_code_is_ok_when_receiving_OK_from_hardware()
{

  fake_wifiModule_send("OK\r\n", 5);

  TEST_ASSERT_EQUAL(WIFI_OK, wifi_command_AT());
}

void test_wifi_command_AT_error_code_is_WIFI_ERROR_RECEIVED_ERROR_when_receiving_nothing_from_hardware()
{
  TEST_ASSERT_EQUAL(WIFI_ERROR_NOT_RECEIVING, wifi_command_AT());
}

void test_wifi_command_AT_error_code_is_WIFI_WIFI_ERROR_RECEIVED_ERROR_when_receiving_ERROR_from_hardware()
{

  fake_wifiModule_send("ERROR", 6);
  TEST_ASSERT_EQUAL(WIFI_ERROR_RECEIVED_ERROR, wifi_command_AT());
}

void test_wifi_command_AT_error_code_is_WIFI_WIFI_ERROR_GARBAGE_when_receiving_GARBAGE_from_hardware()
{
  fake_wifiModule_send("ER\0OR", 6);
  TEST_ASSERT_EQUAL(WIFI_ERROR_RECEIVING_GARBAGE, wifi_command_AT());
}

// check wificonnection OK
// test wrong password or SSID
void test_wifi_join_AP_OK()
{
  fake_wifiModule_send("OK\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_OK, wifi_command_join_AP("correct ssid", "correct Password"));
}
void test_wifi_join_AP_FAIL_because_wrong_ssid_or_password()
{
  fake_wifiModule_send("FAIL\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_FAIL, wifi_command_join_AP("Incorrect ssid", "INcorrect Password"));
}

// check TCP connection OK

void test_wifi_TCP_connection_OK()
{
  fake_wifiModule_send("OK\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_OK, wifi_command_create_TCP_connection("The IP adress", 8000, NULL, NULL));
  // wifi_command_AT();
  fake_wifiModule_send("OK\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_OK, wifi_command_create_TCP_connection("The IP adress", 8000, TCP_Received_callback_func, TEST_BUFFER));
}

void test_wifi_TCP_connection_failed()
{
  fake_wifiModule_send("FAIL\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_FAIL, wifi_command_create_TCP_connection("The IP adress", 8000, NULL, NULL));

}


void string_send_from_TCP_server(char * cArray){
  fake_wifiModule_send("OK\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_OK, wifi_command_create_TCP_connection("The IP adress", 8000, TCP_Received_callback_func, TEST_BUFFER));


  UART_Callback_t callback = uart_init_fake.arg2_val;

    for (int i = 0; i < strlen(cArray); i++)
  {
    callback(cArray[i]);
  }
}
void test_wifi_can_receive(){
  string_send_from_TCP_server("\r\n+IPD,4:1234");
  TEST_ASSERT_EQUAL_STRING("1234",TEST_BUFFER);
}

void test_wifi_TCP_still_receive_If_it_receives_garbage_before_hand(){
  string_send_from_TCP_server("thisIsAlotOfGarbage\r\n+IPD,4:1234");
  TEST_ASSERT_EQUAL_STRING("1234",TEST_BUFFER);
  string_send_from_TCP_server("\r\n\r\n+IPD,4:1234");
  TEST_ASSERT_EQUAL_STRING("1234",TEST_BUFFER);

}

void test_wifi_TCP_check_that_callbackFunc_is_called(){
string_send_from_TCP_server("\r\n+IPD,1:1");
TEST_ASSERT_EQUAL(1,TCP_Received_callback_func_fake.call_count);
}

void test_wifi_TCP_check_that_callbackFunc_is_not_called_when_message_is_not_yet_sent(){

char * cArray ="\r\n+IPD,1:";
  fake_wifiModule_send("OK\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_OK, wifi_command_create_TCP_connection("The IP adress", 8000, TCP_Received_callback_func, TEST_BUFFER));


  UART_Callback_t callback = uart_init_fake.arg2_val;

    for (int i = 0; i < strlen(cArray); i++)
  {
    callback(cArray[i]);
  }
TEST_ASSERT_EQUAL(0,TCP_Received_callback_func_fake.call_count);
callback('a');
TEST_ASSERT_EQUAL(1,TCP_Received_callback_func_fake.call_count);

}
// test that callback is called when receiving
void test_wifi_TCP_check_what_happens_if_it_receives_part_of_the_prefix_before_Hand(){
  string_send_from_TCP_server("thisIsAlo++IPD,Garbage\r\n+IPD,4:1234");
  TEST_ASSERT_EQUAL_STRING("1234",TEST_BUFFER);
  TEST_ASSERT_EQUAL(1, TCP_Received_callback_func_fake.call_count);
  string_send_from_TCP_server("\r\n\r\n+IPD,4:1234");
  TEST_ASSERT_EQUAL_STRING("1234",TEST_BUFFER);
}


void array_send_from_TCP_server(char * cArray, int length){
  fake_wifiModule_send("OK\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_OK, wifi_command_create_TCP_connection("The IP adress", 8000, TCP_Received_callback_func, TEST_BUFFER));


  UART_Callback_t callback = uart_init_fake.arg2_val;

    for (int i = 0; i < length; i++)
  {
    callback(cArray[i]);
  }
}

void test_wifi_zeroes_in_data(){

  array_send_from_TCP_server("\0+IPD,4:B2\0a",12);
  TEST_ASSERT_EQUAL_STRING("B2\0a",TEST_BUFFER);
  TEST_ASSERT_EQUAL(1, TCP_Received_callback_func_fake.call_count);
}

void test_wifi_send(){
  fake_wifiModule_send("OK\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_OK,wifi_command_TCP_transmit("sendThis",8));
  TEST_ASSERT_EQUAL_STRING("AT+CIPSEND=8\r\n",uart_send_string_blocking_fake.arg1_val );


  TEST_ASSERT_EQUAL_STRING("sendThis",uart_send_array_blocking_fake.arg1_val );
}
void test_wifi_send_data_with_a_zero(){
  fake_wifiModule_send("OK\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_OK,wifi_command_TCP_transmit("sendTh\0s!A!",11));
  TEST_ASSERT_EQUAL_STRING("AT+CIPSEND=11\r\n",uart_send_string_blocking_fake.arg1_val );


  TEST_ASSERT_EQUAL_STRING("sendTh\0s!A!",uart_send_array_blocking_fake.arg1_val );
  TEST_ASSERT_EQUAL_INT8_ARRAY("sendTh\0s!A!",uart_send_array_blocking_fake.arg1_val ,11);
}

void test_wifi_quit_AP(){
    fake_wifiModule_send("OK\r\n", 5);
  TEST_ASSERT_EQUAL(WIFI_OK,wifi_command_quit_AP());

  TEST_ASSERT_EQUAL_STRING("AT+CWQAP\r\n", uart_send_string_blocking_fake.arg1_val);

}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_wifi_default_callback_func_is_null);
  RUN_TEST(test_wifi_command_AT_sends_correct_stuff_to_uart);
  RUN_TEST(test_wifi_command_AT_error_code_is_ok_when_receiving_OK_from_hardware);
  RUN_TEST(test_wifi_command_AT_error_code_is_WIFI_ERROR_RECEIVED_ERROR_when_receiving_nothing_from_hardware);
  RUN_TEST(test_wifi_command_AT_error_code_is_WIFI_WIFI_ERROR_RECEIVED_ERROR_when_receiving_ERROR_from_hardware);
  RUN_TEST(test_wifi_command_AT_error_code_is_WIFI_WIFI_ERROR_GARBAGE_when_receiving_GARBAGE_from_hardware);
  RUN_TEST(test_wifi_join_AP_OK);
  RUN_TEST(test_wifi_join_AP_FAIL_because_wrong_ssid_or_password);
  RUN_TEST(test_wifi_TCP_connection_OK);
  RUN_TEST(test_wifi_TCP_connection_failed);
  RUN_TEST(test_wifi_can_receive);
  RUN_TEST(test_wifi_TCP_still_receive_If_it_receives_garbage_before_hand);
  RUN_TEST(test_wifi_TCP_check_that_callbackFunc_is_called);
  RUN_TEST(test_wifi_TCP_check_that_callbackFunc_is_not_called_when_message_is_not_yet_sent);
  RUN_TEST(test_wifi_TCP_check_what_happens_if_it_receives_part_of_the_prefix_before_Hand);
  RUN_TEST(test_wifi_zeroes_in_data);
  RUN_TEST(test_wifi_send);
  RUN_TEST(test_wifi_send_data_with_a_zero);
  RUN_TEST(test_wifi_quit_AP);
  return UNITY_END();
}