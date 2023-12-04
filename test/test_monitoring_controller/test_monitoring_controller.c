#include "../fff.h"
#include "connection_control.h"
#include "fakes.h"
#include "monitoring_system_control.h"
#include "message_builder.h"
#include "unity.h"

#define TEST_MONITORING_CONTROLLER
DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(bool, connection_control_transmit, Package);
FAKE_VALUE_FUNC(Package, message_builder_build_monitor, int8_t, uint8_t,
                uint8_t, uint16_t);

void setUp(void) {
  FFF_RESET_HISTORY();
  RESET_FAKE(dht11_init);
  RESET_FAKE(dht11_get);
  RESET_FAKE(light_init);
  RESET_FAKE(light_read);
  RESET_FAKE(pc_comm_send_string_blocking);
}

void test_monitoring_system_control_init(void) {
  monitoring_system_control_init();

  TEST_ASSERT_EQUAL_INT(1, dht11_init_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, light_init_fake.call_count);
}

void test_monitoring_system_control_execute_successful(void) {
  light_read_fake.return_val = 1000;

  monitoring_system_control_execute();
  TEST_ASSERT_EQUAL_INT(1, dht11_get_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, light_read_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, message_builder_build_monitor_fake.call_count);

  TEST_ASSERT_EQUAL(DHT11_OK, dht11_get_fake.return_val_history[0]);
  TEST_ASSERT_EQUAL_INT(1, connection_control_transmit_fake.call_count);
}

void tearDown(void) {}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_monitoring_system_control_init);
  RUN_TEST(test_monitoring_system_control_execute_successful);
  return UNITY_END();
}