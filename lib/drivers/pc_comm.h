/**
 * @file pc_comm.h
 * @author Laurits Ivar
 * @brief PC Communication interface header using UART.
 * @version 0.2
 * @date 2023-08-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "uart.h"
#include "stddef.h"

/**
 * @brief Define which USART module is used for PC communication.
 * 
 */
#define USART_PC_COMM USART_0

/**
 * @brief Type definition for PC communication data received callback.
 * 
 * @param char The character that was received from the PC.
 */
typedef void (*pc_comm_callback_t)(char);

/**
 * @brief Initialize the PC communication module. If you dont want to receive data from the PC, just set the callbak to NULL. Example: pc_comm_init(115200, NULL);
 * 
 * @param baudrate Desired communication speed with the PC.
 * @param char_received_callback Callback function executed when a character is received from the PC. This can be NULL
 */
void pc_comm_init(uint32_t baudrate, pc_comm_callback_t char_received_callback);

/**
 * @brief Send an array of data to the PC using a blocking method.
 * 
 * @param data Pointer to the array of data to send.
 * @param length Length of the data to send.
 */
void pc_comm_send_array_blocking(uint8_t *data, uint16_t length);

/**
 * @brief Send a null-terminated string to the PC using a blocking method.
 * 
 * @param string The null-terminated string to send.
 */
void pc_comm_send_string_blocking(char *string);

/**
 * @brief Send an array of data to the PC without blocking the execution.
 * 
 * @param str Pointer to the array of data to send.
 * @param len Length of the data to send.
 */
void pc_comm_send_array_nonBlocking(uint8_t *str, uint16_t len);
