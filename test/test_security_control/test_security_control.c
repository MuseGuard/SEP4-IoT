#include "../fff.h"
#include "fakes.h"
#include "message_builder.h"
#include "security_system_control.h"
#include "unity.h"

#define TEST_SECURITY_CONTROLLER

/* FAKE_VOID_FUNC(free, void *);
FAKE_VOID_FUNC(memcpy, void *, const void *, size_t);
FAKE_VOID_FUNC(malloc, void *, size_t); */

void setUp(void) {
  FFF_RESET_HISTORY();
  RESET_FAKE(_delay_ms);
  RESET_FAKE(buzzer_beep);
  RESET_FAKE(display_int);
  RESET_FAKE(pir_init);
  RESET_FAKE(pc_comm_send_string_blocking);
}

void tearDown(void) {
  // Clean up after each test if needed
}
void test_pir_init(void) { TEST_ASSERT_EQUAL(1, 1); }

void test_security_system_control_activate(void) {
  security_system_control_activate();
  TEST_ASSERT_EQUAL(1, pir_init_fake.call_count);
}

void test_security_system_control_check_pin_code(void) {
  uint8_t input_code[4] = {1, 2, 3, 4};
  bool result = security_system_control_check_pin_code(input_code);
  TEST_ASSERT_TRUE(result);
}

void test_security_system_control_check_pin_code_fail(void) {
  uint8_t input_code[4] = {1, 2, 3, 5};
  bool result = security_system_control_check_pin_code(input_code);
  TEST_ASSERT_FALSE(result);
}

void test_security_system_control_toggle_status_local_successful(void) {
  char *message = security_system_control_toggle_status(false);
  TEST_ASSERT_EQUAL_STRING("SSCLocal", message);
}

void test_security_system_control_toggle_status_remote_successful(void) {
  char *message = security_system_control_toggle_status(true);
  TEST_ASSERT_EQUAL_STRING("SSCRemote", message);
}

// TODO ASK
void test_security_system_control_change_pin_code(void) {
  uint8_t input_code[4] = {1, 2, 3, 5};
  char *message = security_system_control_change_pin_code(input_code);
  TEST_ASSERT_EQUAL_STRING("NewPIN=1235", message);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_pir_init);
  RUN_TEST(test_security_system_control_activate);
  RUN_TEST(test_security_system_control_check_pin_code);
  RUN_TEST(test_security_system_control_check_pin_code_fail);
  RUN_TEST(test_security_system_control_toggle_status_local_successful);
  RUN_TEST(test_security_system_control_toggle_status_remote_successful);
  //   RUN_TEST(test_security_system_control_change_pin_code);

  return UNITY_END();
}