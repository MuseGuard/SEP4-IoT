#include "../fff.h"
#include "connection_control.h"
#include "fakes.h"
#include "monitoring_system_control.h"
#include "message_builder.h"
#include "unity.h"

#define TEST_MONITORING_CONTROLLER
DEFINE_FFF_GLOBALS;

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
    dht11_get_fake.return_val = DHT11_OK;
    light_read_fake.return_val = 1000;

    //char expected_message[] = "Test message";

    char *result = monitoring_system_control_execute();

    TEST_ASSERT_EQUAL_INT(1, dht11_get_fake.call_count);
    TEST_ASSERT_EQUAL_INT(1, light_read_fake.call_count);

    TEST_ASSERT_EQUAL_INT(1, message_builder_build_monitor_fake.call_count);


}
void test_monitoring_system_control_execute_fail(void) {
    dht11_get_fake.return_val = DHT11_FAIL;

    char *result = monitoring_system_control_execute();

    TEST_ASSERT_EQUAL_INT(1, dht11_get_fake.call_count);

    TEST_ASSERT_EQUAL_STRING("Error reading DHT11 sensor!", result);
}
void test_monitoring_system_control_execute_dht_success_light_failure(void) {
    dht11_get_fake.return_val = DHT11_OK;

    light_read_fake.return_val = 0;

    char *result = monitoring_system_control_execute();

    TEST_ASSERT_EQUAL_INT(1, dht11_get_fake.call_count);
    TEST_ASSERT_EQUAL_INT(1, light_read_fake.call_count);
}

void test_monitoring_system_control_execute_light_success_dht_failure(void) {
    dht11_get_fake.return_val = DHT11_FAIL;
    light_read_fake.return_val = 1000;
    char expected_message[] = "Error reading DHT11 sensor!";
    message_builder_build_monitor_fake.return_val = expected_message;

    char *result = monitoring_system_control_execute();

    TEST_ASSERT_EQUAL_INT(1, dht11_get_fake.call_count);
    TEST_ASSERT_EQUAL_INT(0, light_read_fake.call_count);

    TEST_ASSERT_EQUAL_INT(0, message_builder_build_monitor_fake.call_count);


    TEST_ASSERT_EQUAL_STRING(expected_message, result);
}
void tearDown(void) {}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_monitoring_system_control_init);
  //RUN_TEST(test_monitoring_system_control_execute_successful);
  RUN_TEST(test_monitoring_system_control_execute_fail);
  //RUN_TEST(test_monitoring_system_control_execute_dht_success_light_failure);
  RUN_TEST(test_monitoring_system_control_execute_light_success_dht_failure);
  
  return UNITY_END();
}