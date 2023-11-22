#include "../fff.h"
#include "unity.h"
#include "pc_comm.h"

#include "uart.h"
#include <stdio.h>
#include <stdint.h>

#define TEST_PC_COMM_WIN

DEFINE_FFF_GLOBALS

void setUp(void)
{
  pc_comm_init(115200, NULL);
}
void tearDown(void) {}
FAKE_VOID_FUNC(uart_init, USART_t , uint32_t , UART_Callback_t );
FAKE_VOID_FUNC(uart_send_array_blocking, USART_t, uint8_t*, uint16_t);
FAKE_VOID_FUNC(uart_send_string_blocking, USART_t, char*);
FAKE_VOID_FUNC(uart_send_array_nonBlocking, USART_t, uint8_t *, uint16_t);

void test_pc_comm_default_callback_func_is_null()
{

  TEST_ASSERT_EQUAL(NULL, uart_init_fake.arg2_val);
  TEST_ASSERT_EQUAL(1,uart_init_fake.call_count);
}

//test baudrate
void test_pc_comm_baudrates()
{

  TEST_ASSERT_EQUAL(115200, uart_init_fake.arg1_val);
  pc_comm_init(9600, NULL);
  TEST_ASSERT_EQUAL(9600, uart_init_fake.arg1_val);
}

void test_pc_comm_send_blocking(){
char data[] ="hejsa" ;
  pc_comm_send_array_blocking((uint8_t *) data, 4 );
TEST_ASSERT_EQUAL(data, uart_send_array_blocking_fake.arg1_val);
}



void test_pc_comm_string_blocking(){
char data[] ="hejsa" ;

pc_comm_send_string_blocking(data );
TEST_ASSERT_EQUAL_STRING(data, uart_send_string_blocking_fake.arg1_val);




}



// Test that it sendst stuff nonBlocking. 
char message_string[1024];
void message(){
for (int i = 0; i < 300; i++)
{
  UNITY_OUTPUT_CHAR(' ');
}


for (int i = 0; i < strlen(message_string); i++)
{
  UNITY_OUTPUT_CHAR(message_string[i]);
}

for (int i = 0; i < 300; i++)
{
  UNITY_OUTPUT_CHAR(' ');
}


}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_pc_comm_default_callback_func_is_null);
  RUN_TEST(test_pc_comm_baudrates);
  RUN_TEST(test_pc_comm_send_blocking);

TEST_MESSAGE("m e s s a g e :1:_:PASS\n");//TEST_MESSAGE("m e s s a g e :1:_:PASS\n");



  RUN_TEST(test_pc_comm_string_blocking);


  return UNITY_END();
}