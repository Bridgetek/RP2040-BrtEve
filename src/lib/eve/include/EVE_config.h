/**
* @file EVE_config.h
* @brief This header file will contain all required EVE's setting
*
* @author BridgeTek
*
* @date 2019
*/

#ifndef _EVE_CONFIG_H_
#define _EVE_CONFIG_H_
#include "PlatformArduino.h"

// Select the EVE controller type from the supported list in FT8xx.h.
// Note: In FT8xx.h this will lead to the selection of the EVE Programming
// support methods via macros "EVEx_ENABLE" where 'x' depends on the level of
// the EVE device support. Alternatively directly set the EVEx_ENABLE macro
// required. This must be called prior to including FT8xx.h.
// "#define FT8XX_TYPE BT817" is equivelant to having "#define EVE4_ENABLE".
#ifndef FT8XX_TYPE
#define FT8XX_TYPE BT817
#endif

// Definitions used for target display resolution selection
#define	WQVGA	480		// e.g. VM800B with 5" or 4.3" display
#define WVGA 	800		// e.g. ME813A-WH50C or VM816
#define	WSVGA	1024	// e.g. ME817EV with 7" display
// Select the 
#ifndef DISPLAY_RES
#define DISPLAY_RES WVGA
#endif

// Explicitly disable QuadSPI
#ifdef QUADSPI_ENABLE
#undef QUADSPI_ENABLE
#endif

// For IDM2040 SPI bus 0 on pins GPIO2 to GPIO5 and PD on GPIO15
#define RP2040_SPI spi0
#define RP2040_SPI_SCK 2
#define RP2040_SPI_MOSI 3
#define RP2040_SPI_MISO 4
#define RP2040_SPI_CS 5
#define RP2040_SPI_PD 15

// Setup default parameters for various displays.
// These can be overridden for different display modules.
#undef SET_PCLK_FREQ

#if DISPLAY_RES == WQVGA

#define EVE_DISP_WIDTH 480 // Active width of LCD display
#define EVE_DISP_HEIGHT 272 // Active height of LCD display
#define EVE_DISP_HCYCLE 548 // Total number of clocks per line
#define EVE_DISP_HOFFSET 43 // Start of active line
#define EVE_DISP_HSYNC0 0 // Start of horizontal sync pulse
#define EVE_DISP_HSYNC1 41 // End of horizontal sync pulse
#define EVE_DISP_VCYCLE 292 // Total number of lines per screen
#define EVE_DISP_VOFFSET 12 // Start of active screen
#define EVE_DISP_VSYNC0 0 // Start of vertical sync pulse
#define EVE_DISP_VSYNC1 10 // End of vertical sync pulse
#define EVE_DISP_PCLK 5 // Pixel Clock
#define EVE_DISP_SWIZZLE 0 // Define RGB output pins
#define EVE_DISP_PCLKPOL 1 // Define active edge of PCLK
#define EVE_DISP_CSPREAD 0
#define EVE_DISP_DITHER 1

#elif DISPLAY_RES == WVGA

#define EVE_DISP_WIDTH 800 // Active width of LCD display
#define EVE_DISP_HEIGHT 480 // Active height of LCD display
#define EVE_DISP_HCYCLE 928 // Total number of clocks per line
#define EVE_DISP_HOFFSET 88 // Start of active line
#define EVE_DISP_HSYNC0 0 // Start of horizontal sync pulse
#define EVE_DISP_HSYNC1 48 // End of horizontal sync pulse
#define EVE_DISP_VCYCLE 525 // Total number of lines per screen
#define EVE_DISP_VOFFSET 32 // Start of active screen
#define EVE_DISP_VSYNC0 0 // Start of vertical sync pulse
#define EVE_DISP_VSYNC1 3 // End of vertical sync pulse
#define EVE_DISP_PCLK 2 // Pixel Clock
#define EVE_DISP_SWIZZLE 0 // Define RGB output pins
#define EVE_DISP_PCLKPOL 1 // Define active edge of PCLK
#define EVE_DISP_CSPREAD 0
#define EVE_DISP_DITHER 1

#elif DISPLAY_RES == WSVGA

#define EVE_DISP_WIDTH 1024 // Active width of LCD display
#define EVE_DISP_HEIGHT 600 // Active height of LCD display
#define EVE_DISP_HCYCLE 1344 // Total number of clocks per line
#define EVE_DISP_HOFFSET 160 // Start of active line
#define EVE_DISP_HSYNC0 0 // Start of horizontal sync pulse
#define EVE_DISP_HSYNC1 100 // End of horizontal sync pulse
#define EVE_DISP_VCYCLE 635 // Total number of lines per screen
#define EVE_DISP_VOFFSET 23 // Start of active screen
#define EVE_DISP_VSYNC0 0 // Start of vertical sync pulse
#define EVE_DISP_VSYNC1 10 // End of vertical sync pulse
#define EVE_DISP_PCLK 1 // Pixel Clock
#define EVE_DISP_SWIZZLE 0 // Define RGB output pins
#define EVE_DISP_PCLKPOL 1 // Define active edge of PCLK
#define EVE_DISP_CSPREAD 0
#define EVE_DISP_DITHER 1
// Set the PCLK frequency to 51MHz (recommend to use the CMD_PCLKFREQ for easier calculation)
#define SET_PCLK_FREQ
#define EVE_DISP_PCLK_FREQ  0xD12	// set 51MHz (must also define SET_PCLK_FREQ in line above to use this)

#else

#error EVE_DISP_* parameters must be configured.

#endif







#endif /* _EVE_CONFIG_H_ */
