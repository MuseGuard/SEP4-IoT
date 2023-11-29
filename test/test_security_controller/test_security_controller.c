#include "unity.h"
#include "../fff.h"
#include "security_system_controller.h"
#include "package_builder.h"
#include "fakes.h"



#define TEST_SECURITY_CONTROLLER


void setUp(void) {
    FFF_RESET_HISTORY();
    RESET_FAKE(buzzer_beep);
    RESET_FAKE(display_int);
    RESET_FAKE(pir_init);
    RESET_FAKE(wifi_command_TCP_transmit);
    RESET_FAKE(pc_comm_send_string_blocking);
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