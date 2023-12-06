#include "unity.h"
#include "../fff.h"
#include "fakes.h"
#include "buttons_control.h"
#include <stdlib.h>

#define TEST_BUTTONS_CONTROLLER

void setUp(void) {
    RESET_FAKE(buttons_init);
    RESET_FAKE(buttons_1_pressed);
    RESET_FAKE(buttons_2_pressed);
    RESET_FAKE(buttons_3_pressed);
    RESET_FAKE(_delay_ms);
}

void tearDown(void) {}

void test_buttons_control_init(void) {
    TEST_ASSERT_EQUAL(0, buttons_init_fake.call_count);
    buttons_control_init();
    TEST_ASSERT_EQUAL(1, buttons_init_fake.call_count);
}

void test_buttons_control_listen_default(void) {
    uint8_t result = buttons_control_listen();
    TEST_ASSERT_EQUAL_UINT8(0, result);
}
void test_buttons_control_listen_button_1(void) {
    //TEST_ASSERT_EQUAL(0, 1);
    buttons_1_pressed_fake.return_val = 1;
    uint8_t result = buttons_control_listen();
    TEST_ASSERT_EQUAL_UINT8(1, result);
}
void test_buttons_control_listen_button_2(void) {
    //TEST_ASSERT_EQUAL(0, 1);
    buttons_2_pressed_fake.return_val = 1;
    uint8_t result = buttons_control_listen();
    TEST_ASSERT_EQUAL_UINT8(2, result);
}
void test_buttons_control_listen_button_3(void) {
    //TEST_ASSERT_EQUAL(0, 1);

    buttons_3_pressed_fake.return_val = 1;
    uint8_t result = buttons_control_listen();
    TEST_ASSERT_EQUAL_UINT8(3, result);
}

//I just cant get the native for buttons_control_pin_code_input to work


int main(void) {
   UNITY_BEGIN();
    RUN_TEST(test_buttons_control_init);
    RUN_TEST(test_buttons_control_listen_default);
    RUN_TEST(test_buttons_control_listen_button_1);
    RUN_TEST(test_buttons_control_listen_button_2);
    RUN_TEST(test_buttons_control_listen_button_3);



  return UNITY_END();
  }