/**
 * @file wifi.h
 * @author Laurits Ivar
 * @brief ESP8266 WiFi module interface using UART.
 * @version 0.1
 * @date 2023-08-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <stdint.h>
#include "uart.h"

/**
 * @brief Define which USART module is used for WiFi communication.
 * 
 */
#define USART_WIFI USART_3

/**
 * @brief Enumerated list of possible error messages from the WiFi module.
 * 
 */
typedef enum{
    WIFI_OK,                         /**< Command successful. */
    WIFI_FAIL,                       /**< General failure or operation not successful. */
    WIFI_ERROR_RECEIVED_ERROR,       /**< Received an error message from the module. */
    WIFI_ERROR_NOT_RECEIVING,        /**< No data received from the module. */
    WIFI_ERROR_RECEIVING_GARBAGE     /**< Received unintelligible data from the module. */
} WIFI_ERROR_MESSAGE_t;

/**
 * @brief Type definition for WiFi TCP data received callback.
 * 
 */
typedef void (*WIFI_TCP_Callback_t)();

/**
 * @brief Initialize the WiFi module. After it have been initialized it can take up to 4 seconds before its ready. 
 * 
 */
void wifi_init();

/**
 * @brief Send an AT command to the WiFi module to check if it's responsive.
 * 
 * @return WIFI_ERROR_MESSAGE_t Error message based on the response from the module.
 */
WIFI_ERROR_MESSAGE_t wifi_command_AT();

/**
 * @brief Command the WiFi module to join a specific Access Point (AP).
 * 
 * @param ssid Network SSID to join.
 * @param password Password for the SSID.
 * @return WIFI_ERROR_MESSAGE_t Error message based on the response from the module.
 */
WIFI_ERROR_MESSAGE_t wifi_command_join_AP(char *ssid, char *password);

/**
 * @brief Disable echo from the WiFi module.
 * 
 * @return WIFI_ERROR_MESSAGE_t Error message based on the response from the module.
 */
WIFI_ERROR_MESSAGE_t wifi_command_disable_echo();

/**
 * @brief Set the WiFi module to mode 1.
 * 
 * @return WIFI_ERROR_MESSAGE_t Error message based on the response from the module.
 */
WIFI_ERROR_MESSAGE_t wifi_command_set_mode_to_1();

/**
 * @brief Set the WiFi module to operate in single connection mode.
 * 
 * @return WIFI_ERROR_MESSAGE_t Error message based on the response from the module.
 */
WIFI_ERROR_MESSAGE_t wifi_command_set_to_single_Connection();

/**
 * @brief Establish a TCP connection using the WiFi module.
 * 
 * @param IP IP address to connect to.
 * @param port Port number to use for the connection.
 * @param callback_when_message_received Callback executed when a message is received.
 * @param received_message_buffer Buffer to hold the received message.
 * @return WIFI_ERROR_MESSAGE_t Error message based on the response from the module.
 */
WIFI_ERROR_MESSAGE_t wifi_command_create_TCP_connection(char *IP, uint16_t port, WIFI_TCP_Callback_t callback_when_message_received, char *received_message_buffer);

/**
 * @brief Transmit data over an established TCP connection.
 * 
 * @param data Pointer to the data to transmit.
 * @param length Length of the data to transmit.
 * @return WIFI_ERROR_MESSAGE_t Error message based on the response from the module.
 */
WIFI_ERROR_MESSAGE_t wifi_command_TCP_transmit(uint8_t *data, uint16_t length);

/**
 * @brief Disconnect from the current Access Point (AP).
 * 
 * @return WIFI_ERROR_MESSAGE_t Error message based on the response from the module.
 */
WIFI_ERROR_MESSAGE_t wifi_command_quit_AP();

/**
 * @brief  Closes thhe TCP connection
 * @return WIFI_ERROR_MESSAGE_t Error message based on the response from the module.
 */
WIFI_ERROR_MESSAGE_t wifi_command_close_TCP_connection();