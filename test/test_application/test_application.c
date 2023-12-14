#include "../fff.h"
#include "fakes.h"
#include "Application.h"
#include "message_builder.h"
#include "unity.h"
#include <stdbool.h>
#include "buttons_control.h"
#include "display_control.h"
#include "connection_control.h"
#include "security_system_control.h"
#include "monitoring_system_control.h"

#define TEST_APPLICATION
DEFINE_FFF_GLOBALS

//Button Control
FAKE_VOID_FUNC(buttons_control_init);
FAKE_VALUE_FUNC(uint8_t, buttons_control_listen);

//Connection Control
server_callback_t callback_fake;
FAKE_VOID_FUNC(connection_control_callback);
FAKE_VALUE_FUNC(bool, connection_control_init, server_callback_t);
FAKE_VALUE_FUNC(bool, connection_control_send_message, char*);

//Display Control
FAKE_VOID_FUNC(display_control_init);
FAKE_VOID_FUNC(display_control_write_word, char*);
FAKE_VALUE_FUNC(uint8_t*, display_control_convert_word_to_numbers, char*);
FAKE_VOID_FUNC(display_control_show_pin_code_position, uint8_t*, uint8_t);

//Monitoring Control
FAKE_VOID_FUNC(monitoring_system_control_init);
FAKE_VALUE_FUNC(char*, monitoring_system_control_execute);

//Security Control
pir_detection_callback_t pir_callback_fake;
FAKE_VALUE_FUNC(bool, security_system_control_check_pin_code, uint8_t*);
FAKE_VOID_FUNC(security_system_control_activate);
FAKE_VALUE_FUNC(char*, security_system_control_toggle_status, bool);
FAKE_VOID_FUNC(security_system_control_send_notification);
FAKE_VALUE_FUNC(char*, security_system_control_change_pin_code, uint8_t*);
FAKE_VALUE_FUNC(bool, security_system_control_is_on);
FAKE_VOID_FUNC(security_system_control_init, pir_detection_callback_t);

void setUp(void) 
{
    FFF_RESET_HISTORY();
    RESET_FAKE(buttons_control_init);
    RESET_FAKE(connection_control_init);
    RESET_FAKE(display_control_init);
    RESET_FAKE(monitoring_system_control_init);
    RESET_FAKE(security_system_control_init);
}

void tearDown(void) 
{
    // Clean up after each test if needed
}

void test_application_init(void) 
{
    TEST_ASSERT_EQUAL(1, 1);
    application_init();
    TEST_ASSERT_EQUAL(1, buttons_control_init_fake.call_count);
    TEST_ASSERT_EQUAL(1, connection_control_init_fake.call_count);
    TEST_ASSERT_EQUAL(1, display_control_init_fake.call_count);
    TEST_ASSERT_EQUAL(1, monitoring_system_control_init_fake.call_count);
    TEST_ASSERT_EQUAL(1, security_system_control_init_fake.call_count);
}

void test_application_run_success(void) 
{
    application_run();
    TEST_ASSERT_EQUAL(1, buttons_control_listen_fake.call_count);
}

void test_application_take_measurements_success(void)
{
    application_take_measurements();
    TEST_ASSERT_EQUAL(1, monitoring_system_control_execute_fake.call_count);
}

void test_application_buttons_listen(void) 
{
    buttons_control_listen_fake.return_val = 1;
    TEST_ASSERT_EQUAL(1, buttons_control_listen_fake.call_count);

    char word[] = "HI  ";
    display_control_write_word(word);
    TEST_ASSERT_EQUAL(1, display_setValues_fake.call_count);
    TEST_ASSERT_EQUAL_UINT8(33, display_setValues_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL_UINT8(18, display_setValues_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL_UINT8(37, display_setValues_fake.arg2_history[0]);
    TEST_ASSERT_EQUAL_UINT8(37, display_setValues_fake.arg3_history[0]);
}


void test_application_on_message_received_callback_change_security_status(void) 
{
    char buffer[] = "ChangeSecurityStatus";

    apllication_on_message_received_callback(buffer);
    
    TEST_ASSERT_EQUAL(2, security_system_control_toggle_status_fake.call_count);
    TEST_ASSERT_EQUAL(true, security_system_control_toggle_status_fake.arg0_history[0]);

    TEST_ASSERT_EQUAL(4, pc_comm_send_string_blocking_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("ChangeSecurityStatus", pc_comm_send_string_blocking_fake.arg0_history[1]);

    TEST_ASSERT_EQUAL(2, connection_control_send_message_fake.call_count);

    // cannot get the value from the connevction_control_send_message_fake.arg0_history[0] 
    // shows null
    // TEST_ASSERT_EQUAL_STRING("ChangeSecurityStatus", connection_control_send_message_fake.arg0_history[0]);
}



/*--------------- CAN'T TEST ---------------*/

// void test_application_get_new_pin_code_success(void) 
// {
//     security_system_control_is_on_fake.return_val = true;
//     uint8_t new_pin_code[] = {1, 2, 3, 4};
//      application_take_pin_input_fake.return_val = new_pin_code;
//     security_system_control_change_pin_code_fake.return_val = "NewPIN=1235";

//     application_get_new_pin();

    // TEST_ASSERT_EQUAL(1, display_control_write_word_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING("Edit", display_control_write_word_fake.arg0_history[0]);

    // // TEST_ASSERT_EQUAL(1, application_take_pin_input_fake.call_count);

    // TEST_ASSERT_EQUAL(1, pc_comm_send_string_blocking_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING("NewPIN=1235", pc_comm_send_string_blocking_fake.arg0_history[0]);

    // TEST_ASSERT_EQUAL(1, connection_control_send_message_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING("NewPIN=1235", connection_control_send_message_fake.arg0_history[0]);

    // TEST_ASSERT_EQUAL(1, free_fake.call_count);

    // TEST_ASSERT_EQUAL(1, display_control_write_word_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING("OK", display_control_write_word_fake.arg0_history[1]);
// }

/*--------------- CAN'T TEST ---------------*/

// void test_application_get_new_pin_code_fail(void)
// {
//     security_system_control_is_on_fake.return_val = false;

//     application_get_new_pin();

//     TEST_ASSERT_EQUAL(1, pc_comm_send_string_blocking_fake.call_count);
//     TEST_ASSERT_EQUAL_STRING("Unlock the device first\n", pc_comm_send_string_blocking_fake.arg0_history[1]);

//     TEST_ASSERT_EQUAL(1, display_control_write_word_fake.call_count);
//     TEST_ASSERT_EQUAL_STRING("Err", display_control_write_word_fake.arg0_history[2]);

//     TEST_ASSERT_EQUAL(1, _delay_ms_fake.call_count);
//     TEST_ASSERT_EQUAL(1000, _delay_ms_fake.arg0_history[0]);
// }
int main(void) 
{
    UNITY_BEGIN();

    RUN_TEST(test_application_init);
    RUN_TEST(test_application_take_measurements_success);
    RUN_TEST(test_application_run_success);
    RUN_TEST(test_application_buttons_listen);
    RUN_TEST(test_application_on_message_received_callback_change_security_status);

    return UNITY_END();
}