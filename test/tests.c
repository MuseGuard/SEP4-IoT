#include "fff.h"
#include <unity.h>
DEFINE_FFF_GLOBALS
#include <stdbool.h>

// Facking functions

void setUp() { }

void tearDown() {}

void test_nothing() { TEST_ASSERT_EQUAL(1, 1); }


int main(int argc, char const *argv[]) {
  UNITY_BEGIN();
  RUN_TEST(test_nothing);
  return UNITY_END();
}
