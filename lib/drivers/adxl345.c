#include "adxl345.h"
#include "includes.h"

// GND
#define GND_BIT PG1
#define GND_DDR DDRG
#define GND_PORT PORTG

// VCC
#define VCC_BIT PD7
#define VCC_DDR DDRD
#define VCC_PORT PORTD

// CS
#define CS_BIT DDC1
#define CS_DDR DDRC
#define CS_PORT PORTC

// SDO/MISO
#define MISO_BIT PC7 // SDO to MISO (Pin 50)
#define MISO_DDR DDRC
#define MISO_PIN PINC

// SDA/MOSI
#define MOSI_BIT PA6 // SDA/SDI to MOSI
#define MOSI_DDR DDRA
#define MOSI_PORT PORTA

// SCL/Clock
#define SCL_BIT PA4
#define SCL_DDR DDRA
#define SCL_PORT PORTA



// ADXL345 Register Addresses
#define ADXL345_DEVID 0x00
#define ADXL345_POWER_CTL 0x2D
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32

// ADXL345 specific constants
#define ADXL345_RANGE_16G 0x03
#define ADXL345_MEASURE_MODE 0x08

uint8_t spi_transfer(uint8_t data);

void adxl345_init(void)
{
    MOSI_DDR |= (1 << MOSI_BIT);
    SCL_DDR |= (1 << SCL_BIT);
    CS_DDR |= (1 << CS_BIT);

    MISO_DDR &= ~(1 << MISO_BIT);

    // //Chip sellect high, because its not sellected.
    CS_PORT |= (1 << CS_BIT);

    // Ground
    GND_DDR |= (1 << GND_BIT);
    GND_PORT &= ~(1 << GND_BIT);

    // Vcc
    VCC_DDR |= (1 << VCC_BIT);
    VCC_PORT |= (1 << VCC_BIT);

    // Clock is default high.
    SCL_PORT |= (1 << SCL_BIT);

    _delay_ms(20);

    adxl345_write_register(ADXL345_POWER_CTL, ADXL345_MEASURE_MODE);
    adxl345_write_register(ADXL345_DATA_FORMAT, 0b00000101);
}

uint8_t spi_transfer(uint8_t data)
{
    uint8_t received_data = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & (1 << (7 - i)))
        {
            MOSI_PORT |= (1 << MOSI_BIT);
        }
        else
        {
            MOSI_PORT &= ~(1 << MOSI_BIT);
        }

        SCL_PORT &= ~(1 << SCL_BIT);
        _delay_us(1);

        received_data <<= 1;
        if (MISO_PIN & (1 << MISO_BIT))
        {
            received_data |= 1;
        }

        SCL_PORT |= (1 << SCL_BIT);
        _delay_us(1);
    }

    return received_data;
}

void adxl345_write_register(uint8_t reg, uint8_t value)
{
    CS_PORT &= ~(1 << CS_BIT);
    _delay_us(1);

    spi_transfer(reg);
    spi_transfer(value);

    CS_PORT |= (1 << CS_BIT);
}

uint8_t adxl345_read_register(uint8_t reg)
{
    CS_PORT &= ~(1 << CS_BIT);
    spi_transfer(0x80 | reg);
    uint8_t value = spi_transfer(0x00);
    CS_PORT |= (1 << CS_BIT);

    return value;
}

void adxl345_read_xyz(int16_t *x, int16_t *y, int16_t *z)
{
    CS_PORT &= ~(1 << CS_BIT);
    // spi_transfer(0x80 | 0x40 | ADXL345_DATAX0);
    *z = spi_transfer(0xC0 | 0x32);

    *x = spi_transfer(0x00);
    *x |= spi_transfer(0x00) << 8;

    *y = spi_transfer(0x00);
    *y |= spi_transfer(0x00) << 8;

    *z = spi_transfer(0x00);
    *z |= spi_transfer(0x00) << 8;

    CS_PORT |= (1 << CS_BIT);
}
