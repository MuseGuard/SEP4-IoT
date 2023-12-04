#include "../fff.h"
#include "connection_control.h"
#include "fakes.h"
#include "security_system_control.h"
#include "unity.h"
DEFINE_FFF_GLOBALS

#define TEST_CONNECTION_CONTROLLER

void setUp(void) {
  RESET_FAKE(_delay_ms);
  RESET_FAKE(wifi_init);
  RESET_FAKE(wifi_command_join_AP);
  RESET_FAKE(wifi_command_create_TCP_connection);
  RESET_FAKE(pc_comm_send_string_blocking);
  RESET_FAKE(wifi_command_TCP_transmit);
}

void tearDown(void) {}

void test_connection_control_init_success(void) {
  // TEST_ASSERT_EQUAL(1, 0);
  wifi_command_join_AP_fake.return_val = WIFI_OK;
  wifi_command_create_TCP_connection_fake.return_val = WIFI_OK;

  bool result = connection_control_init();

  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(1, wifi_command_join_AP_fake.call_count);
  // TEST_ASSERT_EQUAL_STRING("madinnit", wifi_command_join_AP_fake.arg0_val);
  // TEST_ASSERT_EQUAL_STRING("12345678", wifi_command_join_AP_fake.arg1_val);

  TEST_ASSERT_EQUAL(1, wifi_command_create_TCP_connection_fake.call_count);
  // TEST_ASSERT_EQUAL_STRING("192.168.214.218",
  // wifi_command_create_TCP_connection_fake.arg0_val);
  // TEST_ASSERT_EQUAL_UINT16(23,
  // wifi_command_create_TCP_connection_fake.arg1_val);

  TEST_ASSERT_EQUAL_STRING("Connected to AP!\n",
                           pc_comm_send_string_blocking_fake.arg0_history[0]);
  TEST_ASSERT_EQUAL_STRING("Connected to server!\n",
                           pc_comm_send_string_blocking_fake.arg0_history[1]);

  TEST_ASSERT_EQUAL(1, wifi_command_create_TCP_connection_fake.call_count);
}

void test_connection_control_init_fail_TCP_connection(void) {
  // TEST_ASSERT_EQUAL(1, 0);
  wifi_command_join_AP_fake.return_val = WIFI_OK;
  wifi_command_create_TCP_connection_fake.return_val = WIFI_FAIL;

  bool result = connection_control_init();

  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL(1, wifi_command_join_AP_fake.call_count);
  TEST_ASSERT_EQUAL_STRING("Connected to AP!\n",
                           pc_comm_send_string_blocking_fake.arg0_history[0]);

  TEST_ASSERT_EQUAL(1, wifi_command_create_TCP_connection_fake.call_count);
  TEST_ASSERT_EQUAL_STRING("Error connecting to server!\n",
                           pc_comm_send_string_blocking_fake.arg0_history[1]);

  TEST_ASSERT_EQUAL(1, wifi_command_create_TCP_connection_fake.call_count);
}
void test_connection_control_init_success_AP(void) {
  wifi_command_join_AP_fake.return_val = WIFI_OK;
  wifi_command_create_TCP_connection_fake.return_val = WIFI_FAIL;
  bool result = connection_control_init();

  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL_STRING("Connected to AP!\n",
                           pc_comm_send_string_blocking_fake.arg0_history[0]);
  TEST_ASSERT_EQUAL_STRING("Error connecting to server!\n",
                           pc_comm_send_string_blocking_fake.arg0_history[1]);

  TEST_ASSERT_EQUAL(1, wifi_command_create_TCP_connection_fake.call_count);
}

void test_connection_control_init_fail_AP(void) {
  // TEST_ASSERT_EQUAL(1, 0);
  wifi_command_join_AP_fake.return_val = WIFI_ERROR_NOT_RECEIVING;

  bool result = connection_control_init();

  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL(1, pc_comm_send_string_blocking_fake.call_count);
  TEST_ASSERT_EQUAL_STRING("Error connecting to Wifi AP!\n",
                           pc_comm_send_string_blocking_fake.arg0_val);

  TEST_ASSERT_EQUAL(0, wifi_command_create_TCP_connection_fake.call_count);
}

void test_connection_control_init_success_AP_and_success_server(void) {
  wifi_command_join_AP_fake.return_val = WIFI_OK;
  wifi_command_create_TCP_connection_fake.return_val = WIFI_OK;
  bool result = connection_control_init();

  TEST_ASSERT_FALSE(result);
  TEST_ASSERT_EQUAL_STRING("Connected to AP!\n",
                           pc_comm_send_string_blocking_fake.arg0_history[0]);
  TEST_ASSERT_EQUAL_STRING("Error connecting to server!\n",
                           pc_comm_send_string_blocking_fake.arg0_history[1]);

  TEST_ASSERT_EQUAL(1, wifi_command_create_TCP_connection_fake.call_count);
}

void test_connection_control_transmit_success(void) {
  // TEST_ASSERT_EQUAL(1, 0);
  wifi_command_TCP_transmit_fake.return_val = WIFI_OK;

  Package package = {.data = "T=22.5/H=30/L=600", .size = strlen(package.data)};
  bool result = connection_control_transmit(package);

  TEST_ASSERT_TRUE(result);
}

void test_connection_control_transmit_fail(void) {
  // TEST_ASSERT_EQUAL(1, 0);
  wifi_command_TCP_transmit_fake.return_val = WIFI_FAIL;

  Package package = {.data = "T=22.5/H=30/L=600", .size = strlen(package.data)};
  bool result = connection_control_transmit(package);

  TEST_ASSERT_FALSE(result);
}

void test_connection_control_send_message_fail(void) {
  // TEST_ASSERT_EQUAL(1, 0);
  wifi_command_TCP_transmit_fake.return_val = WIFI_FAIL;

  char message[] = "FAIL";
  bool result = connection_control_send_message(message);

  TEST_ASSERT_FALSE(result);
}

void test_connection_control_send_message_success(void) {
  // TEST_ASSERT_EQUAL(1, 0);
  wifi_command_TCP_transmit_fake.return_val = WIFI_OK;

  char message[] = "SUCCESS";
  bool result = connection_control_send_message(message);

  TEST_ASSERT_TRUE(result);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_connection_control_init_success);
  RUN_TEST(test_connection_control_init_success_AP);
  RUN_TEST(test_connection_control_init_fail_AP);
  RUN_TEST(test_connection_control_init_fail_TCP_connection);
  RUN_TEST(test_connection_control_transmit_success);
  RUN_TEST(test_connection_control_transmit_fail);
  RUN_TEST(test_connection_control_send_message_success);
  RUN_TEST(test_connection_control_send_message_fail);

  return UNITY_END();
}
