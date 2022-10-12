/**
 * @file eve_ui_main.c
 * @brief This source file will contain all required UI functions
 *
 * @author BridgeTek
 *
 * @date 2017
 */

/* INCLUDES ************************************************************************/

#include "../include/eve_ui.h"
#include "../include/eve_ram_g.h"
#include <stdio.h>

/* CONSTANTS ***********************************************************************/

/**
 @brief Allow click on the BridgeTek Logo to make a screenshot.
 */
#define ENABLE_SCREENSHOT 1

/* GLOBAL VARIABLES ****************************************************************/

/* LOCAL VARIABLES *****************************************************************/

/* MACROS **************************************************************************/

/* LOCAL FUNCTIONS / INLINES *******************************************************/

/* FUNCTIONS ***********************************************************************/

void eve_ui_setup()
{
	printf("---------------------------eve_ui_main.ceve_ui_main.ceve_ui_main.ca\n");
	EVE_Init();

	eve_ui_calibrate();

	init_ram_g(0);
}

void eve_ui_splash(char *toast)
{
	EVE_LIB_BeginCoProList();
	EVE_CMD_DLSTART();
	EVE_CLEAR_COLOR_RGB(0, 0, 0);
	EVE_CLEAR(1, 1, 1);
	EVE_COLOR_RGB(255, 255, 255);
	EVE_CMD_TEXT(EVE_DISP_WIDTH / 2, EVE_DISP_HEIGHT / 2,
				 28, EVE_OPT_CENTERX | EVE_OPT_CENTERY, toast);
	EVE_DISPLAY();
	EVE_CMD_SWAP();
	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();
}

void eve_ui_wait(void)
{
	uint8_t key_code = 0;
	uint8_t key_detect = 0;

	eve_ui_splash("Waiting for host...");

	key_detect = eve_ui_read_tag(&key_code);
	if (key_detect)
	{
	}
	eve_ui_arch_sleepms(100);
}

void eve_ui_calibrate()
{
	struct touchscreen_calibration calib;

	eve_ui_arch_flash_calib_init();

	if (eve_ui_arch_flash_calib_read(&calib) != 0)
	{
		EVE_LIB_BeginCoProList();
		EVE_CMD_DLSTART();
		EVE_CLEAR_COLOR_RGB(0, 0, 0);
		EVE_CLEAR(1, 1, 1);
		EVE_COLOR_RGB(255, 255, 255);
		EVE_CMD_TEXT(EVE_DISP_WIDTH / 2, EVE_DISP_HEIGHT / 2,
					 28, EVE_OPT_CENTERX | EVE_OPT_CENTERY,
					 "Please tap on the dots");
		EVE_CMD_CALIBRATE(0);
		EVE_LIB_EndCoProList();
		EVE_LIB_AwaitCoProEmpty();

		calib.transform[0] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_A);
		calib.transform[1] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_B);
		calib.transform[2] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_C);
		calib.transform[3] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_D);
		calib.transform[4] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_E);
		calib.transform[5] = HAL_MemRead32(EVE_REG_TOUCH_TRANSFORM_F);
		eve_ui_arch_flash_calib_write(&calib);
	}
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_A, calib.transform[0]);
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_B, calib.transform[1]);
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_C, calib.transform[2]);
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_D, calib.transform[3]);
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_E, calib.transform[4]);
	HAL_MemWrite32(EVE_REG_TOUCH_TRANSFORM_F, calib.transform[5]);
}

void eve_ui_screenshot()
{
#ifdef ENABLE_SCREENSHOT
	uint32_t img_end_address = malloc_ram_g(EVE_DISP_WIDTH * EVE_DISP_HEIGHT * 2);
	uint8_t buffer[256];
	int i, j;

	// Write screenshot into RAM_G
	EVE_LIB_BeginCoProList();
	EVE_CMD_DLSTART();
	EVE_CMD_SNAPSHOT(img_end_address);
	EVE_LIB_EndCoProList();
	EVE_LIB_AwaitCoProEmpty();

	printf("ARGB start\n"); // Use this marker to identify the start of the image.
	for (i = 0; i < (EVE_DISP_WIDTH * 2) * EVE_DISP_HEIGHT; i += sizeof(buffer))
	{
		EVE_LIB_ReadDataFromRAMG(buffer, sizeof(buffer), img_end_address + i);
		for (j = 0; j < sizeof(buffer); j++)
		{
			printf("%c", buffer[j]);
		}
	}
	printf("ARGB end\n"); // Marker to identify the end of the image.

	free_ram_g(img_end_address);

	eve_ui_splash("Screenshot completed...");
	eve_ui_arch_sleepms(2000);

#endif // ENABLE_SCREENSHOT
}

void eve_ui_play_sound(uint8_t sound, uint8_t volume)
{
	HAL_MemWrite8(EVE_REG_VOL_SOUND, volume);
	HAL_MemWrite8(EVE_REG_SOUND, sound);
	HAL_MemWrite8(EVE_REG_PLAY, 1);
}

uint8_t eve_ui_read_tag(uint8_t *key)
{
	uint8_t Read_tag;
	uint8_t key_detect = 0;

	Read_tag = HAL_MemRead8(EVE_REG_TOUCH_TAG);

	if (!(HAL_MemRead16(EVE_REG_TOUCH_RAW_XY) & 0x8000))
	{
		key_detect = 1;
		*key = Read_tag;
	}

	return key_detect;
}
