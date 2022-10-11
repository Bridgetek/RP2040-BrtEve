/**
 * @file eve_ui_images.c
 * @brief This source file will contain all required images functions
 *
 * @author BridgeTek
 *
 * @date 2017
 */

#include "../../eve/include/EVE_config.h"
#include "../../eve/include/EVE.h"

#include "../include/eve_ui.h"
#include "../include/eve_ram_g.h"

uint32_t eve_ui_jpg_image_size(const uint8_t *image_data, uint16_t *width, uint16_t *height)
{
	uint16_t img_width = 0;
	uint16_t img_height = 0;
	uint8_t *pCh = (uint8_t *)image_data;

	// Read in raw JPEG encoded image data to find width and height of image.
	while (1)
	{
		if (*pCh == 0xff)
		{
			pCh++;
			if (*pCh == 0xc0)
			{
				// Found SOF0
				pCh++;
				pCh++;
				pCh++;
				pCh++;
				img_height = *pCh++;
				img_height <<= 8;
				img_height |= (*pCh++);
				img_width = *pCh++;
				img_width <<= 8;
				img_width |= (*pCh++);
				break;
			}
		}
		pCh++;
	}

#ifdef BIG_ENDIAN
	*width = img_width;
	*height = img_height;
#else // LITTLE_ENDIAN
	*width = (img_width >> 8) | (img_width << 8);
	*height = (img_height >> 8) | (img_height << 8);
#endif

	return img_width * 2 * img_height;
}

uint32_t eve_ui_load_argb2(const uint8_t *image_data, uint32_t image_size, uint8_t image_handle,
						   uint16_t img_width, uint16_t img_height)
{
	uint32_t img_offset;

	img_offset = malloc_ram_g(image_size);
	if (img_offset)
	{
		eve_ui_arch_write_ram_from_pm(image_data, image_size, img_offset);

		EVE_LIB_BeginCoProList();
		EVE_CMD_DLSTART();
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		EVE_BITMAP_HANDLE(image_handle);
		EVE_BITMAP_SOURCE(img_offset);
		EVE_BITMAP_LAYOUT(EVE_FORMAT_ARGB2, img_width, img_height);
		EVE_BITMAP_LAYOUT_H((img_width) >> 10, img_height >> 9);
		EVE_BITMAP_SIZE(EVE_FILTER_NEAREST, EVE_WRAP_BORDER, EVE_WRAP_BORDER,
						img_width, img_height);
		EVE_BITMAP_SIZE_H(img_width >> 9, img_height >> 9);
		EVE_END();

		EVE_DISPLAY();
		EVE_CMD_SWAP();
		EVE_LIB_EndCoProList();
		EVE_LIB_AwaitCoProEmpty();

		img_offset += (img_width * img_height);
	}

	return img_offset;
}

uint32_t eve_ui_load_argb1555(const uint8_t *image_data, uint32_t image_size, uint8_t image_handle,
							  uint16_t img_width, uint16_t img_height)
{
	uint32_t img_offset;

	img_offset = malloc_ram_g(image_size);
	if (img_offset)
	{
		eve_ui_arch_write_ram_from_pm(image_data, image_size, img_offset);

		EVE_LIB_BeginCoProList();
		EVE_CMD_DLSTART();
		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		EVE_BITMAP_HANDLE(image_handle);
		EVE_BITMAP_SOURCE(img_offset);
		EVE_BITMAP_LAYOUT(EVE_FORMAT_ARGB1555, img_width * 2, img_height);
		EVE_BITMAP_LAYOUT_H((img_width * 2) >> 10, img_height >> 9);
		EVE_BITMAP_SIZE(EVE_FILTER_NEAREST, EVE_WRAP_BORDER, EVE_WRAP_BORDER,
						img_width, img_height);
		EVE_BITMAP_SIZE_H(img_width >> 9, img_height >> 9);
		EVE_END();

		EVE_DISPLAY();
		EVE_CMD_SWAP();
		EVE_LIB_EndCoProList();
		EVE_LIB_AwaitCoProEmpty();

		img_offset += (img_width * img_height);
	}

	return img_offset;
}

uint32_t eve_ui_load_jpg(const uint8_t *image_data, uint8_t handle,
						 uint16_t *width, uint16_t *height)
{
	uint32_t img_offset;
	uint32_t img_expanded_size;
	uint16_t img_width = 0;
	uint16_t img_height = 0;
	uint8_t *pData = (uint8_t *)image_data;

	img_expanded_size = eve_ui_jpg_image_size(image_data, &img_width, &img_height);

	img_offset = malloc_ram_g(img_expanded_size);
	if (img_offset)
	{
		uint8_t buf[128];
		int8_t flag;
		int i;

		flag = 0;
		EVE_LIB_BeginCoProList();
		EVE_CMD_LOADIMAGE(img_offset, 0);
		// Send raw JPEG encoded image data to coprocessor. It will be decoded
		// as the data is received.
		while (flag != 2)
		{
			memcpy(buf, pData, sizeof(buf));
			for (i = 0; i < sizeof(buf); i++)
			{
				if (buf[i] == 0xff)
				{
					flag = 1;
				}
				else
				{
					if (flag == 1)
					{
						if (buf[i] == 0xd9)
						{
							flag = 2;
							i++;
							break;
						}
					}
					flag = 0;
				}
			}
			EVE_LIB_WriteDataToCMD(buf, (i + 3) & (~3));
			pData += i;
		};

		EVE_LIB_EndCoProList();
		EVE_LIB_AwaitCoProEmpty();

		if (width)
			*width = img_width;
		if (height)
			*height = img_height;

		EVE_LIB_BeginCoProList();
		EVE_CMD_DLSTART();

		EVE_BEGIN(EVE_BEGIN_BITMAPS);
		EVE_BITMAP_HANDLE(handle);
		EVE_BITMAP_SOURCE(img_offset);
		EVE_BITMAP_LAYOUT(EVE_FORMAT_RGB565, img_width * 2, img_height);
		EVE_BITMAP_LAYOUT_H((img_width * 2) >> 10, img_height >> 9);
		EVE_BITMAP_SIZE(EVE_FILTER_NEAREST, EVE_WRAP_BORDER, EVE_WRAP_BORDER,
						img_width, img_height);
		EVE_BITMAP_SIZE_H(img_width >> 9, img_height >> 9);
		EVE_END();

		EVE_DISPLAY();
		EVE_CMD_SWAP();
		EVE_LIB_EndCoProList();
		EVE_LIB_AwaitCoProEmpty();

		img_offset += (img_width * img_height);
	}

	return img_offset;
}
