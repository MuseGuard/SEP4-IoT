/**
 * @file uart.h
 * @author Laurits Ivar
 * @brief UART communication interface header.
 * @version 0.1
 * @date 2023-08-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <stdint.h>

/**
 * @brief Enumeration for available USART modules.
 * 
 */
typedef enum {
    USART_0,    /**< USART module 0 */
    USART_1,    /**< USART module 1 */
    USART_2,    /**< USART module 2 */
    USART_3     /**< USART module 3 */
} USART_t;

/**
 * @brief Type definition for UART received data callback.
 * 
 * @param data The byte of data that was received.
 */
typedef void (*UART_Callback_t)(uint8_t);

/**
 * @brief Initialize the specified USART module.
 * 
 * @param usart The USART module to be initialized.
 * @param baudrate Desired communication speed.
 * @param callback Callback function for received data.
 */
void uart_init(USART_t usart, uint32_t baudrate, UART_Callback_t callback);

/**
 * @brief Send a single byte over UART using blocking method.
 * 
 * @param usart The USART module to use.
 * @param data The byte of data to send.
 */
void uart_send_blocking(USART_t usart,uint8_t data);

/**
 * @brief Send an array of data over UART without blocking.
 * 
 * @param usart The USART module to use.
 * @param str Pointer to the array of data to send.
 * @param len Length of the data to send.
 */
void uart_send_array_nonBlocking(USART_t usart,  uint8_t *str, uint16_t len);

/**
 * @brief Send an array of data over UART using blocking method.
 * 
 * @param usart The USART module to use.
 * @param data Pointer to the array of data to send.
 * @param length Length of the data to send.
 */
void uart_send_array_blocking(USART_t usart, uint8_t *data, uint16_t length);

/**
 * @brief Send a string over UART using blocking method.
 * 
 * @param usart The USART module to use.
 * @param data Null-terminated string to send.
 */
void uart_send_string_blocking(USART_t usart, char *data);

/**
 * @brief Get the current received data callback for the specified USART module.
 * 
 * @param usart The USART module.
 * @return UART_Callback_t The callback function currently in use.
 */
UART_Callback_t uart_get_rx_callback(USART_t usart);

