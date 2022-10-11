/**
* @file eve_ui_mcu_rp2040.c
* @brief This source file will contain all required RP2040 MCU functions
*
* @author BridgeTek
*
* @date 2017
*/

// Guard against being used for incorrect CPU type.
#if defined(PLATFORM_RP2040)

/* INCLUDES ************************************************************************/

#include "../../include/eve_ui.h"

#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

/* CONSTANTS ***********************************************************************/

/**
 * @brief Location in flash to store touchscreen configuration.
 */
#define FLASH_OFFSET_CONFIG (256 * 1024)

/* GLOBAL VARIABLES ****************************************************************/

/* LOCAL VARIABLES *****************************************************************/

/* MACROS **************************************************************************/

/* LOCAL FUNCTIONS / INLINES *******************************************************/

/**
 * @brief Functions used to store calibration data in flash.
 */
//@{
int8_t eve_ui_arch_flash_calib_init(void)
{
    return 0;
}

int8_t eve_ui_arch_flash_calib_write(struct touchscreen_calibration *calib)
{
#if (PICO_FLASH_SIZE_BYTES < FLASH_OFFSET_CONFIG + FLASH_SECTOR_SIZE)
#error Configuration written above top of flash
#endif
    // Data to write to flash must be aligned to a flash page
    uint8_t config[FLASH_PAGE_SIZE] __aligned(FLASH_PAGE_SIZE);
    uint32_t ints = save_and_disable_interrupts();

	calib->key = VALID_KEY_TOUCHSCREEN;
    memset(config, 0xff, FLASH_PAGE_SIZE);
    memcpy(config, calib, sizeof(struct touchscreen_calibration));

    flash_range_erase(FLASH_OFFSET_CONFIG, FLASH_SECTOR_SIZE);
    flash_range_program(FLASH_OFFSET_CONFIG, (const uint8_t *)config, FLASH_PAGE_SIZE);
    restore_interrupts (ints);

    return 0;
}

int8_t eve_ui_arch_flash_calib_read(struct touchscreen_calibration *calib)
{
    struct touchscreen_calibration *p = (struct touchscreen_calibration *)(XIP_BASE + FLASH_OFFSET_CONFIG);
	if (p->key == VALID_KEY_TOUCHSCREEN)
    {
        memcpy(calib, p, sizeof(struct touchscreen_calibration));
        return 0;
    }

    return -2;
}
//@}

void eve_ui_arch_write_cmd_from_flash(const uint8_t *ImgData, uint32_t length)
{
	uint32_t offset = 0;
	uint8_t ramData[512];
	uint8_t *flash_addr = XIP_BASE + (uint8_t *)ImgData;
	uint32_t left;

	while (offset < length)
	{
		memcpy(ramData, flash_addr, 512);

		if (length - offset < 512)
		{
			left = length - offset;
		}
		else
		{
			left = 512;
		}
		EVE_LIB_WriteDataToCMD(ramData, left);
		offset += left;
		flash_addr += left;
	};
}

void eve_ui_arch_write_ram_from_flash(const uint8_t *ImgData, uint32_t length, uint32_t dest)
{
	uint32_t offset = 0;
	uint8_t ramData[512];
	uint8_t *flash_addr = XIP_BASE + (uint8_t *)ImgData;
	uint32_t left;

	while (offset < length)
	{
		memcpy(ramData, flash_addr, 512);

		if (length - offset < 512)
		{
			left = length - offset;
		}
		else
		{
			left = 512;
		}
		EVE_LIB_WriteDataToRAMG(ramData, left, dest);
		offset += left;
		flash_addr += left;
		dest += left;
	};
}

void eve_ui_arch_write_cmd_from_pm(const uint8_t *ImgData, uint32_t length)
{
	uint32_t offset = 0;
	uint8_t ramData[512];
	uint8_t *pm_addr = (uint8_t *)(ImgData);
	uint32_t left;

	while (offset < length)
	{
		memcpy(ramData, pm_addr, 512);

		if (length - offset < 512)
		{
			left = length - offset;
		}
		else
		{
			left = 512;
		}
		EVE_LIB_WriteDataToCMD(ramData, left);
		offset += left;
		pm_addr += left;
	};
}

void eve_ui_arch_write_ram_from_pm(const uint8_t *ImgData, uint32_t length, uint32_t dest)
{
	uint32_t offset = 0;
	uint8_t ramData[512];
	uint8_t *pm_addr = (uint8_t *)(ImgData);
	uint32_t left;

	while (offset < length)
	{
		memcpy(ramData, pm_addr, 512);

		if (length - offset < 512)
		{
			left = length - offset;
		}
		else
		{
			left = 512;
		}
		EVE_LIB_WriteDataToRAMG(ramData, left, dest);
		offset += left;
		pm_addr += left;
		dest += left;
	};
}

void eve_ui_arch_sleepms(uint32_t ms)
{
	sleep_ms(ms);
}

/* FUNCTIONS ***********************************************************************/

#endif /* defined(PLATFORM_RP2040) */
