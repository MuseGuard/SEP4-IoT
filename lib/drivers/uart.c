#ifndef EXCLUDE_UART


#include "uart.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include <string.h>

#include "includes.h"

static UART_Callback_t usart0_rx_callback = NULL;
static UART_Callback_t usart1_rx_callback = NULL;
static UART_Callback_t usart2_rx_callback = NULL;
static UART_Callback_t usart3_rx_callback = NULL;
UART_Callback_t uart_get_rx_callback(USART_t usart){
    switch (usart)
    {
    case USART_0:
        return usart0_rx_callback;
        break;
    case USART_1:
        return usart1_rx_callback;
        break;
    case USART_2:
        return usart2_rx_callback;
        break;
    case USART_3:
        return usart3_rx_callback;
        break;
    default:
    return 0;
        break;
    }

}
#ifndef WINDOWS_TEST
// This is the ISR for USART0 Receive Complete
#ifndef TARGET_TEST
ISR(USART0_RX_vect)
{
    // If a valid callback has been set, call it
    if (usart0_rx_callback != NULL)
    {
        // Call the callback function with the received data
        usart0_rx_callback(UDR0);
    }
}
#endif

ISR(USART1_RX_vect)
{
    // If a valid callback has been set, call it
    if (usart1_rx_callback != NULL)
    {
        // Call the callback function with the received data
        usart1_rx_callback(UDR1);
    }
}

ISR(USART2_RX_vect)
{
    // If a valid callback has been set, call it
    if (usart2_rx_callback != NULL)
    {
        // Call the callback function with the received data
        usart2_rx_callback(UDR2);
    }
}

ISR(USART3_RX_vect)
{

    // If a valid callback has been set, call it
    if (usart3_rx_callback != NULL)
    {
        // Call the callback function with the received data
        usart3_rx_callback(UDR3);
    }
}
#endif
inline static void uart_init_usart0(uint16_t ubrr, UART_Callback_t callback)
{
    // Enable transmitter, receiver and the transmit interrupt
    UCSR0B = (1 << TXEN0) | (1<< RXEN0);
 //   
 //   UCSR0B = (1 << TXEN0) | (1 << RXEN0)// Enable USART0 transmitter Enable USART0 receiver

    //  frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    // The Baud Rate must be set after the transmitter is enabled
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    if (callback != NULL)
    {
        usart0_rx_callback = callback;

        // Enable the USART Receive Complete interrupt
        UCSR0B |= (1 << RXCIE0);
    }
}

inline static void uart_init_usart1(uint16_t ubrr, UART_Callback_t callback)
{
    UCSR1B = (1 << TXEN1)    // Enable USART0 transmitter
             | (1 << RXEN1); // Enable USART0 receiver

    //  frame format: 8 data bits, 1 stop bit, no parity
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);

    // The Baud Rate must be set after the transmitter is enabled
    UBRR1H = (uint8_t)(ubrr >> 8);
    UBRR1L = (uint8_t)ubrr;

    // Enable transmitter, receiver and the transmit interrupt

    if (callback != NULL)
    {
        usart1_rx_callback = callback;

        // Enable the USART Receive Complete interrupt
        UCSR1B |= (1 << RXCIE1);
    }
}

inline static void uart_init_usart2(uint16_t ubrr, UART_Callback_t callback)
{

    // Enable transmitter, receiver and the transmit interrupt
    UCSR2B = (1 << TXEN2)    // Enable USART0 transmitter
             | (1 << RXEN2); // Enable USART0 receiver

    //  frame format: 8 data bits, 1 stop bit, no parity
    UCSR2C = (1 << UCSZ21) | (1 << UCSZ20);

    // The Baud Rate must be set after the transmitter is enabled
    UBRR2H = (uint8_t)(ubrr >> 8);
    UBRR2L = (uint8_t)ubrr;

    if (callback != NULL)
    {
        usart2_rx_callback = callback;

        // Enable the USART Receive Complete interrupt
        UCSR2B |= (1 << RXCIE2);
    }
}

inline static void uart_init_usart3(uint16_t ubrr, UART_Callback_t callback)
{

    // Enable transmitter, receiver and the transmit interrupt
    UCSR3B = (1 << TXEN3)    // Enable USART0 transmitter
             | (1 << RXEN3); // Enable USART0 receiver

    //  frame format: 8 data bits, 1 stop bit, no parity
    UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);

    // The Baud Rate must be set after the transmitter is enabled
    UBRR3H = (uint8_t)(ubrr >> 8);
    UBRR3L = (uint8_t)ubrr;

    if (callback != NULL)
    {
        usart3_rx_callback = callback;

        // Enable the USART Receive Complete interrupt
        UCSR3B |= (1 << RXCIE3);
        
    }
}

void uart_init(USART_t usart, uint32_t baudrate, UART_Callback_t callback)
{
    cli();

    uint16_t ubrr = (F_CPU / (16ULL * baudrate)) - 1;
    if (ubrr == 7)
        ubrr = 8; // ugly hack because of rounding error..

    switch (usart)
    {
    case USART_0:
        uart_init_usart0(ubrr, callback);
        break;
    case USART_1:
        uart_init_usart1(ubrr, callback);
        break;
    case USART_2:
        uart_init_usart2(ubrr, callback);
        break;
    case USART_3:
        uart_init_usart3(ubrr, callback);
        break;
    default:
        // Handle error: invalid USART choice
        break;
    }
    sei();
}

void uart_send_blocking(USART_t usart, uint8_t data)
{

    switch (usart)
    {
    case USART_0:
        // Wait for the transmit buffer to be empty
        while (!(UCSR0A & (1 << UDRE0)))
        {
            // Do nothing - this is a blocking function
        }
        // Write the data to the transmit buffer
        UDR0 = data;
        break;
    case USART_1:
        // Wait for the transmit buffer to be empty
        while (!(UCSR1A & (1 << UDRE1)))
        {
            // Do nothing - this is a blocking function
        }
        // Write the data to the transmit buffer
        UDR1 = data;
        break;
    case USART_2:
        // Wait for the transmit buffer to be empty
        while (!(UCSR2A & (1 << UDRE2)))
        {
            // Do nothing - this is a blocking function
        }
        // Write the data to the transmit buffer
        UDR2 = data;
        break;
    case USART_3:
        // Wait for the transmit buffer to be empty
        while (!(UCSR3A & (1 << UDRE3)))
        {
            // Do nothing - this is a blocking function
        }
        // Write the data to the transmit buffer
        UDR3 = data;
        break;
    default:
        // Handle error: invalid USART choice
        break;
    }
}

void uart_send_array_blocking(USART_t usart, uint8_t *data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        uart_send_blocking(usart, data[i]);
    }
}



void uart_send_string_blocking(USART_t usart, char *data){
    uart_send_array_blocking(usart, (uint8_t*)data, strlen(data));
}

static volatile uint8_t *usart0_transmit_buffer;
static volatile uint16_t usart0_transmit_index;
static volatile uint16_t usart0_transmit_length;
static volatile uint8_t usart0_transmission_in_progress = 0;

static volatile uint8_t *usart1_transmit_buffer;
static volatile uint16_t usart1_transmit_index;
static volatile uint16_t usart1_transmit_length;
static volatile uint8_t usart1_transmission_in_progress = 0;

static volatile uint8_t *usart2_transmit_buffer;
static volatile uint16_t usart2_transmit_index;
static volatile uint16_t usart2_transmit_length;
static volatile uint8_t usart2_transmission_in_progress = 0;

static volatile uint8_t *usart3_transmit_buffer;
static volatile uint16_t usart3_transmit_index;
static volatile uint16_t usart3_transmit_length;
static volatile uint8_t usart3_transmission_in_progress = 0;

void uart_send_array_nonBlocking(USART_t usart, uint8_t *str, uint16_t len)
{


    while (usart0_transmission_in_progress)
    {
    }

    cli(); // Disable global interrupts to avoid conflicts during variable update
    switch (usart)
    {
    case USART_0:
        // Set the transmit buffer, index and length
        usart0_transmit_buffer = str;
        usart0_transmit_index = 0;
        usart0_transmit_length = len;

        // Set the transmission_in_progress flag
        usart0_transmission_in_progress = 1;

        // Enable the Data Register Empty Interrupt
        UCSR0B |= (1 << UDRIE0); // Enable USART0 data register empty interrupt
        break;
    case USART_1:
        // Set the transmit buffer, index and length
        usart1_transmit_buffer = str;
        usart1_transmit_index = 0;
        usart1_transmit_length = len;

        // Set the transmission_in_progress flag
        usart1_transmission_in_progress = 1;

        // Enable the Data Register Empty Interrupt
        UCSR1B |= (1 << UDRIE1); // Enable USART0 data register empty interrupt
        break;
    case USART_2:
        // Set the transmit buffer, index and length
        usart2_transmit_buffer = str;
        usart2_transmit_index = 0;
        usart2_transmit_length = len;

        // Set the transmission_in_progress flag
        usart2_transmission_in_progress = 1;

        // Enable the Data Register Empty Interrupt
        UCSR2B |= (1 << UDRIE2); // Enable USART0 data register empty interrupt
        break;
    case USART_3:
        // Set the transmit buffer, index and length
        usart3_transmit_buffer = str;
        usart3_transmit_index = 0;
        usart3_transmit_length = len;

        // Set the transmission_in_progress flag
        usart3_transmission_in_progress = 1;

        // Enable the Data Register Empty Interrupt
        UCSR3B |= (1 << UDRIE3); // Enable USART0 data register empty interrupt
        break;
    default:
        // Handle error: invalid USART choice
        break;
    }

    sei(); // Enable global interrupts after updating variables
}

#ifndef WINDOWS_TEST

#ifndef TARGET_TEST

ISR(USART0_UDRE_vect)
{
    // Check if we have more data to send
    if (usart0_transmit_index < usart0_transmit_length)
    {
        UDR0 = usart0_transmit_buffer[usart0_transmit_index++]; // Send next character and increment index
    }
    else
    {
        // Disable the Data Register Empty Interrupt when transmission is complete
        UCSR0B &= ~(1 << UDRIE0);            // Disable USART0 data register empty interrupt
        usart0_transmission_in_progress = 0; // Clear the transmission_in_progress flag
    }
}
#endif

ISR(USART1_UDRE_vect)
{
    // Check if we have more data to send
    if (usart1_transmit_index < usart1_transmit_length)
    {
        UDR1 = usart1_transmit_buffer[usart1_transmit_index++]; // Send next character and increment index
    }
    else
    {
        // Disable the Data Register Empty Interrupt when transmission is complete
        UCSR1B &= ~(1 << UDRIE1);            // Disable USART0 data register empty interrupt
        usart1_transmission_in_progress = 0; // Clear the transmission_in_progress flag
    }
}

ISR(USART2_UDRE_vect)
{
    // Check if we have more data to send
    if (usart2_transmit_index < usart2_transmit_length)
    {
        UDR2 = usart2_transmit_buffer[usart0_transmit_index++]; // Send next character and increment index
    }
    else
    {
        // Disable the Data Register Empty Interrupt when transmission is complete
        UCSR2B &= ~(1 << UDRIE2);            // Disable USART0 data register empty interrupt
        usart2_transmission_in_progress = 0; // Clear the transmission_in_progress flag
    }
}

ISR(USART3_UDRE_vect)
{
    // Check if we have more data to send
    if (usart3_transmit_index < usart3_transmit_length)
    {
        UDR3 = usart3_transmit_buffer[usart3_transmit_index++]; // Send next character and increment index
    }
    else
    {
        // Disable the Data Register Empty Interrupt when transmission is complete
        UCSR3B &= ~(1 << UDRIE3);            // Disable USART0 data register empty interrupt
        usart3_transmission_in_progress = 0; // Clear the transmission_in_progress flag
    }
}

#endif

#endif//EXCLUDE_UART