/**
* @file eve_ui.h
* @brief This header file will contain all required EVE's UI functions
*
* @author BridgeTek
*
* @date 2019
*/

#ifndef _EVE_COMMON_H
#define _EVE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../../eve/include/EVE_config.h"
#include "../../eve/include/EVE.h"
#include "../../eve/include/HAL.h"

/**
 @brief Structure to hold touchscreen calibration settings.
 @details This is used to store the touchscreen calibration settings persistently
 in Flash and identify if the calibration needs to be re-performed.
 */
struct touchscreen_calibration {
	uint32_t key; // VALID_KEY_TOUCHSCREEN
	uint32_t transform[6];
};

/**
 @brief Key for identifying if touchscreen calibration values are programmed into
 datalogger memory in the Flash.
 */
#define VALID_KEY_TOUCHSCREEN 0xd72f91a3

/* User Interface helper routines.
 * Definitions of these functions are in the source/eve_ui_ext_font.c file.
 */
void eve_ui_calibrate();
void eve_ui_play_sound(uint8_t sound, uint8_t volume);
uint8_t eve_ui_read_tag(uint8_t *key);

void eve_ui_setup();
void eve_ui_wait(void);
void eve_ui_splash(char *msg);
void eve_ui_screenshot(void);

/* Font helper routines.
 * Definitions of these functions are in the source/eve_ui_ext_font.c file.
 */
uint32_t eve_ui_load_font(const uint8_t *font_data, uint32_t font_size, uint8_t font_handle);
uint32_t eve_ui_load_font2(uint8_t first, const uint8_t *font_data, uint32_t font_size, uint8_t font_handle);

/* Image helper routines.
 * Definitions of these functions are in the source/eve_ui_images.c file.
 */
uint32_t eve_ui_jpg_image_size(const uint8_t *image_data, uint16_t *width, uint16_t *height);
uint32_t eve_ui_load_argb2(const uint8_t *image_data, uint32_t image_size, uint8_t image_handle,
		uint16_t img_width, uint16_t img_height);
uint32_t eve_ui_load_argb1555(const uint8_t *image_data, uint32_t image_size, uint8_t image_handle,
		uint16_t img_width, uint16_t img_height);
uint32_t eve_ui_load_jpg(const uint8_t *image_data, uint8_t handle,
		uint16_t *width, uint16_t *height);

/* Platform specific functions. 
 * Definition of these functions are in the eve_ui/ports/<platform> folders.
 */
int8_t eve_ui_arch_flash_calib_init(void);
int8_t eve_ui_arch_flash_calib_write(struct touchscreen_calibration *calib);
int8_t eve_ui_arch_flash_calib_read(struct touchscreen_calibration *calib);
void eve_ui_arch_write_cmd_from_flash(const uint8_t *ImgData, uint32_t length);
void eve_ui_arch_write_ram_from_flash(const uint8_t *ImgData, uint32_t length, uint32_t dest);
void eve_ui_arch_write_ram_from_pm(const uint8_t *ImgData, uint32_t length, uint32_t dest);
void eve_ui_arch_sleepms(uint32_t ms);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* _EVE_COMMON_H */
