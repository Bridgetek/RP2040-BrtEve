/**
* @file EVE_MCU_RP2040.c
* @brief This source file will contain all required RP2040 MCU functions
*
* @author BridgeTek
*
* @date 2022
*/

// Guard against being used for incorrect CPU type.
#if defined(ARDUINO_ARCH_RP2040)

#pragma message "Compiling " __FILE__ " for Raspberry Pi pico RP2040"

#include <string.h>
#include <stdint.h> // for Uint8/16/32 and Int8/16/32 data types
#include <stdio.h>
#include <machine/endian.h>

#include "../../include/MCU.h"
#include "../../include/EVE_config.h"

#include "pico/stdlib.h"
#include "hardware/spi.h"

// This is not yet supported
#undef QUADSPI_ENABLE

#ifndef RP2040_SPI
// Default pins to match Bridgetek IDM2040-7A board.
const uint pd_pin = 15;
const uint cs_pin = 5;
const uint sck_pin = 2;
const uint mosi_pin = 3;
const uint miso_pin = 4;
spi_inst_t *spi_port = spi0;
#else
// Application configured pins
const uint pd_pin = RP2040_SPI_PD;
const uint cs_pin = RP2040_SPI_CS;
const uint sck_pin = RP2040_SPI_SCK;
const uint mosi_pin = RP2040_SPI_MOSI;
const uint miso_pin = RP2040_SPI_MISO;
spi_inst_t *spi_port = RP2040_SPI;
#endif

// This is the MCU specific section and contains the functions which talk to the
// PIC registers. If porting the code to a different PIC or to another MCU, these
// should be modified to suit the registers of the selected MCU.

// ------------------- MCU specific initialisation  ----------------------------
void MCU_Init(void)
{
    // Initialise CS (SPI Chip Select) pin high
    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);

	// Initialise PD (Power Down) pin high
    gpio_init(pd_pin);
    gpio_set_dir(pd_pin, GPIO_OUT);
    gpio_put(pd_pin, 1);

    // Initialize SPI port at 1 MHz
    spi_init(spi_port, 1000 * 1000);

	// Set SPI format
    spi_set_format( spi_port, 		// SPI instance
                    8,      		// Number of bits per transfer
                    SPI_CPOL_0,		// Polarity (CPOL)
                    SPI_CPHA_0,		// Phase (CPHA)
                    SPI_MSB_FIRST);

    // Initialize SPI pin functions
    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(mosi_pin, GPIO_FUNC_SPI);
    gpio_set_function(miso_pin, GPIO_FUNC_SPI);

#if defined QUADSPI_ENABLE

#if (defined EVE2_ENABLE || defined EVE3_ENABLE || defined EVE4_ENABLE)
	/* Initialize IO2 and IO3 pad/pin for quad settings */
#endif

#endif// QUADSPI_ENABLE
}

void MCU_Setup(void)
{
#if defined QUADSPI_ENABLE

#if (defined EVE2_ENABLE || defined EVE3_ENABLE || defined EVE4_ENABLE)
	// Turn on EVE quad-SPI for FT81x devices.
	MCU_CSlow();
	MCU_SPIWrite24(MCU_htobe32((EVE_REG_SPI_WIDTH << 8) | (1 << 31)));
	MCU_SPIWrite8(2);
	MCU_CShigh();
#endif//(defined EVE2_ENABLE || defined EVE3_ENABLE || defined EVE4_ENABLE)

#endif// QUADSPI_ENABLE
}

// ########################### GPIO CONTROL ####################################

// --------------------- Chip Select line low ----------------------------------
inline void MCU_CSlow(void)
{
    gpio_put(cs_pin, 0);
}  

// --------------------- Chip Select line high ---------------------------------
inline void MCU_CShigh(void)
{
    gpio_put(cs_pin, 1);
}

// -------------------------- PD line low --------------------------------------
inline void MCU_PDlow(void)
{
    gpio_put(pd_pin, 0);
}

// ------------------------- PD line high --------------------------------------
inline void MCU_PDhigh(void)
{
    gpio_put(pd_pin, 1);
}

// --------------------- SPI Send and Receive ----------------------------------

uint8_t MCU_SPIRead8(void)
{
	uint8_t DataRead = 0;

	spi_read_blocking(spi_port, 0, &DataRead, 1);

	return DataRead;
}

void MCU_SPIWrite8(uint8_t DataToWrite)
{
    spi_write_blocking(spi_port, &DataToWrite, 1);
}

uint16_t MCU_SPIRead16(void)
{
	uint16_t DataRead = 0;

	spi_read_blocking(spi_port, 0, (uint8_t *)&DataRead, 2);

	return DataRead;
}

void MCU_SPIWrite16(uint16_t DataToWrite)
{
	spi_write_blocking(spi_port, (uint8_t *)&DataToWrite, 2);
}

uint32_t MCU_SPIRead24(void)
{
	uint32_t DataRead = 0;

	spi_read_blocking(spi_port, 0, (uint8_t *)&DataRead, 3);

	return DataRead;
}

void MCU_SPIWrite24(uint32_t DataToWrite)
{
	spi_write_blocking(spi_port, (uint8_t *)&DataToWrite, 3);
}

uint32_t MCU_SPIRead32(void)
{
	uint32_t DataRead = 0;

	spi_read_blocking(spi_port, 0, (uint8_t *)&DataRead, 4);

	return DataRead;
}

void MCU_SPIWrite32(uint32_t DataToWrite)
{
	spi_write_blocking(spi_port, (uint8_t *)&DataToWrite, 4);
}

void MCU_SPIWrite(const uint8_t *DataToWrite, uint32_t length)
{
	spi_write_blocking(spi_port, DataToWrite, length);
}

void MCU_Delay_20ms(void)
{
	sleep_ms(20);
}

void MCU_Delay_500ms(void)
{
	sleep_ms(500);
}

// RP2040 is Little Endian.
// Use toolchain defined functions.
uint16_t MCU_htobe16(uint16_t h)
{
	return __bswap16(h);
}

uint32_t MCU_htobe32(uint32_t h)
{
	return __bswap32(h);
}

uint16_t MCU_htole16(uint16_t h)
{
	return h;
}

uint32_t MCU_htole32(uint32_t h)
{
	return h;
}

uint16_t MCU_be16toh(uint16_t h)
{
	return __bswap16(h);
}

uint32_t MCU_be32toh(uint32_t h)
{
	return __bswap32(h);
}

uint16_t MCU_le16toh(uint16_t h)
{
	return h;
}

uint32_t MCU_le32toh(uint32_t h)
{
	return h;
}

#endif /* defined(ARDUINO_ARCH_RP2040) */
