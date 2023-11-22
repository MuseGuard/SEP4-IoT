#include "unity.h"
#include "../fff.h"
#include "connection_controller.h"
#include "wifi.h"
#include "pc_comm.h"
DEFINE_FFF_GLOBALS

#define TEST_CONNECTION_CONTROLLER


FAKE_VOID_FUNC(wifi_init);
FAKE_VOID_FUNC(_delay_ms, int);
FAKE_VALUE_FUNC(WIFI_ERROR_MESSAGE_t, wifi_command_join_AP, char*, char*);
FAKE_VALUE_FUNC(WIFI_ERROR_MESSAGE_t, wifi_command_create_TCP_connection, char*, uint16_t, WIFI_TCP_Callback_t, char*);
FAKE_VOID_FUNC(pc_comm_send_string_blocking, char*);
FAKE_VALUE_FUNC(WIFI_ERROR_MESSAGE_t, wifi_command_TCP_transmit, uint8_t*, uint16_t);

void setUp(void) {
    RESET_FAKE(_delay_ms);
    RESET_FAKE(wifi_init);
    RESET_FAKE(wifi_command_join_AP);
    RESET_FAKE(wifi_command_create_TCP_connection);
    RESET_FAKE(pc_comm_send_string_blocking);
    RESET_FAKE(wifi_command_TCP_transmit);
}

void tearDown(void) {
    
}

void test_connection_controller_init_Success(void) {
    wifi_command_join_AP_fake.return_val = WIFI_OK;
    wifi_command_create_TCP_connection_fake.return_val = WIFI_OK;

    bool result = connection_controller_init();

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(1, wifi_command_join_AP_fake.call_count);
    // TEST_ASSERT_EQUAL_STRING("madinnit", wifi_command_join_AP_fake.arg0_val);
    // TEST_ASSERT_EQUAL_STRING("12345678", wifi_command_join_AP_fake.arg1_val);


    TEST_ASSERT_EQUAL(1, wifi_command_create_TCP_connection_fake.call_count);
    //TEST_ASSERT_EQUAL_STRING("192.168.214.218", wifi_command_create_TCP_connection_fake.arg0_val);
    //TEST_ASSERT_EQUAL_UINT16(23, wifi_command_create_TCP_connection_fake.arg1_val);
}

void test_connection_controller_init_Fail_Wifi(void) {
    wifi_command_join_AP_fake.return_val = WIFI_ERROR_NOT_RECEIVING;

    bool result = connection_controller_init();

    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_EQUAL(1, pc_comm_send_string_blocking_fake.call_count);
    TEST_ASSERT_EQUAL_STRING("Error connecting to Wifi AP!\n", pc_comm_send_string_blocking_fake.arg0_val);

    TEST_ASSERT_EQUAL(0, wifi_command_create_TCP_connection_fake.call_count);
}

void test_connection_controller_transmit(void) {
    Package package;

    connection_controller_transmit(package);

    TEST_ASSERT_EQUAL(1, wifi_command_TCP_transmit_fake.call_count);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_connection_controller_init_Success);
    RUN_TEST(test_connection_controller_init_Fail_Wifi);
    RUN_TEST(test_connection_controller_transmit);

    return UNITY_END();
}
