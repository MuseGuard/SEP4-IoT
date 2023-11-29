#include "unity.h"
#include "../fff.h"
#include "monitoring_system_control.h"
#include "package_builder.h"
#include "connection_controller.h"
#include "fakes.h"

#define TEST_MONITORING_CONTROLLER
DEFINE_FFF_GLOBALS;


void setUp(void) {
    // Reset fake functions before each test
    FFF_RESET_HISTORY();
    RESET_FAKE(dht11_init);
    RESET_FAKE(dht11_get);
    RESET_FAKE(light_init);
    RESET_FAKE(light_read);
    RESET_FAKE(pc_comm_send_string_blocking);
}

void test_monitoring_system_controller_init(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

void tearDown(void) {
    // Clean up after each test if needed
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_monitoring_system_controller_init);
    return UNITY_END();
}