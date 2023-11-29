#include "unity.h"
#include "../fff.h"
#include "monitoring_system_control.h"
#include "dht11.h"
#include "package_builder.h"
#include "connection_controller.h"
#include "light.h"

#define TEST_MONITORING_CONTROLLER
// Define FFF globals and mock functions
DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(dht11_init);
FAKE_VALUE_FUNC(DHT11_ERROR_MESSAGE_t, dht11_get, uint8_t*, uint8_t*, uint8_t*, uint8_t*);
FAKE_VOID_FUNC(light_init);
FAKE_VALUE_FUNC(uint16_t, light_read);
FAKE_VALUE_FUNC(Package, package_builder_build_monitor, int8_t, uint8_t, uint8_t, uint16_t);
FAKE_VALUE_FUNC(bool, connection_controller_transmit, Package);
FAKE_VOID_FUNC(pc_comm_send_string_blocking, char*);

void setUp(void) {
    // Reset fake functions before each test
    FFF_RESET_HISTORY();
    RESET_FAKE(dht11_init);
    RESET_FAKE(dht11_get);
    RESET_FAKE(light_init);
    RESET_FAKE(light_read);
    RESET_FAKE(package_builder_build_monitor);
    RESET_FAKE(connection_controller_transmit);
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