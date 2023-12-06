#include "unity.h"
#include "../fff.h"
#include "fakes.h"
#include "display_control.h"


#define TEST_DISPLAY_CONTROLLER

void test_display_control_convert_word_to_numbers() {
    char word1[] = "Hello";
    uint8_t expected1[] = {17, 14, 21, 21, 24}; //'H', 'E', 'L', 'L', 'O'
    uint8_t *result1 = display_control_convert_word_to_numbers(word1);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected1, result1, strlen(word1));
}
void test_display_control_convert_word_to_numbers_special_signs(){
    char word2[] = "12-ab_"; //giberish
    uint8_t expected2[] = {1, 2, 36, 10, 11, 39};
    uint8_t *result2 = display_control_convert_word_to_numbers(word2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected2, result2, strlen(word2));
}
void test_display_control_convert_word_to_numbers_empty_spaces(){
    char word2[] = " "; //giberish
    uint8_t expected2[] = {37};
    uint8_t *result2 = display_control_convert_word_to_numbers(word2);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected2, result2, strlen(word2));
}


void test_display_control_write_word(void) {
    char word[] = "ABCD";

    display_control_write_word(word);

    TEST_ASSERT_EQUAL(2, display_setValues_fake.call_count);
    TEST_ASSERT_EQUAL_UINT8(10, display_setValues_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL_UINT8(11, display_setValues_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL_UINT8(12, display_setValues_fake.arg2_history[0]);
    TEST_ASSERT_EQUAL_UINT8(13, display_setValues_fake.arg3_history[0]);
}


void setUp(void) {
    FFF_RESET_HISTORY();
    RESET_FAKE(display_setValues);
}
void tearDown(void) {
}

void test_display_controller_init(void) {
    display_control_init();
    TEST_ASSERT_EQUAL(1, display_init_fake.call_count);
}
void test_display_control_show_pin_code_position_0(void) {
    uint8_t pin_code[] = { 1, 2, 3, 4 };

    display_control_show_pin_code_position(pin_code, 0);

    TEST_ASSERT_EQUAL_UINT8(39, display_setValues_fake.arg0_val);
    TEST_ASSERT_EQUAL_UINT8(2, display_setValues_fake.arg1_val);
    TEST_ASSERT_EQUAL_UINT8(3, display_setValues_fake.arg2_val);
    TEST_ASSERT_EQUAL_UINT8(4, display_setValues_fake.arg3_val);
    TEST_ASSERT_EQUAL(1, display_setValues_fake.call_count);
}
void test_display_control_show_pin_code_position_1(void) {
     uint8_t pin_code[] = { 1, 2, 3, 4 };
    display_control_show_pin_code_position(pin_code, 1);

    TEST_ASSERT_EQUAL_UINT8(1, display_setValues_fake.arg0_val);
    TEST_ASSERT_EQUAL_UINT8(39, display_setValues_fake.arg1_val);
    TEST_ASSERT_EQUAL_UINT8(3, display_setValues_fake.arg2_val);
    TEST_ASSERT_EQUAL_UINT8(4, display_setValues_fake.arg3_val);
    TEST_ASSERT_EQUAL(1, display_setValues_fake.call_count);
}
void test_display_control_show_pin_code_position_2(void){
    uint8_t pin_code[] = { 1, 2, 3, 4 };

    display_control_show_pin_code_position(pin_code, 2);

    TEST_ASSERT_EQUAL_UINT8(1, display_setValues_fake.arg0_val);
    TEST_ASSERT_EQUAL_UINT8(2, display_setValues_fake.arg1_val);
    TEST_ASSERT_EQUAL_UINT8(39, display_setValues_fake.arg2_val);
    TEST_ASSERT_EQUAL_UINT8(4, display_setValues_fake.arg3_val);
    TEST_ASSERT_EQUAL(1, display_setValues_fake.call_count);
}

void test_display_control_show_pin_code_position_3(void){
    uint8_t pin_code[] = { 1, 2, 3, 4 };

    display_control_show_pin_code_position(pin_code, 3);

    TEST_ASSERT_EQUAL_UINT8(1, display_setValues_fake.arg0_val);
    TEST_ASSERT_EQUAL_UINT8(2, display_setValues_fake.arg1_val);
    TEST_ASSERT_EQUAL_UINT8(3, display_setValues_fake.arg2_val);
    TEST_ASSERT_EQUAL_UINT8(39, display_setValues_fake.arg3_val);
    TEST_ASSERT_EQUAL(1, display_setValues_fake.call_count);

}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_display_controller_init);
    RUN_TEST(test_display_control_convert_word_to_numbers);
    RUN_TEST(test_display_control_convert_word_to_numbers_special_signs);
    RUN_TEST(test_display_control_convert_word_to_numbers_empty_spaces);
    RUN_TEST(test_display_control_write_word);
    RUN_TEST(test_display_control_show_pin_code_position_0);
    RUN_TEST(test_display_control_show_pin_code_position_1);
    RUN_TEST(test_display_control_show_pin_code_position_2);
    RUN_TEST(test_display_control_show_pin_code_position_3);

  
  return UNITY_END();
}