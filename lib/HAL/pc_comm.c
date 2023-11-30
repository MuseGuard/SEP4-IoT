#include "pc_comm.h"
#include "includes.h"


void pc_comm_init(uint32_t baudrate, pc_comm_callback_t char_received_callback){
    uart_init(USART_PC_COMM, baudrate, (UART_Callback_t)char_received_callback);
}


void pc_comm_send_array_blocking(uint8_t *data, uint16_t length){
    uart_send_array_blocking(USART_PC_COMM, data, length);
}

void pc_comm_send_string_blocking(char * string){
    uart_send_string_blocking(USART_PC_COMM, string);
}


void pc_comm_send_array_nonBlocking(uint8_t * data, uint16_t length){
    uart_send_array_nonBlocking(USART_PC_COMM, data,  length);
}