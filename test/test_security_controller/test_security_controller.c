#include "unity.h"
#include "../fff.h"
#include "security_system_controller.h"
#include "package_builder.h"
#include "pir.h"
#include "wifi.h"


#define TEST_SECURITY_CONTROLLER


// Define FFF globals and mock functions
DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(cli);
FAKE_VOID_FUNC(sei);
FAKE_VOID_FUNC(buzzer_beep);
FAKE_VOID_FUNC(display_int, int);
FAKE_VOID_FUNC(pir_init, pir_callback_t);
FAKE_VALUE_FUNC(Package, package_builder_build_motion_detected);
FAKE_VALUE_FUNC(bool, connection_controller_transmit, Package);
FAKE_VALUE_FUNC(WIFI_ERROR_MESSAGE_t, wifi_command_TCP_transmit, uint8_t*, uint16_t);
FAKE_VOID_FUNC(pc_comm_send_string_blocking, char*);
FAKE_VALUE_FUNC(uint8_t*, buttons_control_pin_code_input);

void setUp(void) {
    FFF_RESET_HISTORY();
    RESET_FAKE(buzzer_beep);
    RESET_FAKE(display_int);
    RESET_FAKE(pir_init);
    RESET_FAKE(package_builder_build_motion_detected);
    RESET_FAKE(connection_controller_transmit);
    RESET_FAKE(wifi_command_TCP_transmit);
    RESET_FAKE(pc_comm_send_string_blocking);
    RESET_FAKE(buttons_control_pin_code_input);
}

void tearDown(void) {
    // Clean up after each test if needed
}
void test_pir_init(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_pir_init);

    return UNITY_END();
}