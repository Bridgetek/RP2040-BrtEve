/**
* @file eve_ui_ext_font.c
* @brief This source file will contain all required font functions
*
* @author BridgeTek
*
* @date 2017
*/

#include "../../eve/include/EVE_config.h"
#include "../../eve/include/EVE.h"

#include "../include/eve_ui.h"
#include "../include/eve_ram_g.h"

static const EVE_GPU_FONT_HEADER *font_pm_pointers[32] = {0};

uint8_t eve_ui_font_header(uint8_t font_handle, EVE_GPU_FONT_HEADER *font_hdr)
{
	uint32_t font_root;
	uint32_t font_offset;

	if (font_handle < 32)
	{
		if (font_pm_pointers[font_handle])
		{
			memcpy(font_hdr, font_pm_pointers[font_handle], sizeof(EVE_GPU_FONT_HEADER));
			return 0;
		}
		else if (font_handle >= 16)
		{
			EVE_LIB_ReadDataFromRAMG((void *)&font_root, sizeof(uint32_t), EVE_ROMFONT_TABLEADDRESS);

			font_offset = font_root + ((font_handle - 16) * sizeof(EVE_GPU_FONT_HEADER));
			EVE_LIB_ReadDataFromRAMG((void *)font_hdr, sizeof(EVE_GPU_FONT_HEADER), font_offset);

			return 0;
		}
	}
	return -1;
}

uint8_t eve_ui_font_size(uint8_t font_handle, uint16_t *width, uint16_t *height)
{
	EVE_GPU_FONT_HEADER font_hdr;

	if (eve_ui_font_header(font_handle, &font_hdr) == 0)
	{
		*width = font_hdr.FontWidthInPixels;
		*height = font_hdr.FontHeightInPixels;
		return 0;
	}
	return -1;
}

uint8_t eve_ui_font_char_width(uint8_t font_handle, char ch)
{
	EVE_GPU_FONT_HEADER font_hdr;
	uint8_t width = 0;

	if (eve_ui_font_header(font_handle, &font_hdr) == 0)
	{
		width = font_hdr.FontWidth[(int)ch];
	}
	return width;
}

uint8_t eve_ui_font_string_width(uint8_t font_handle, const char *str)
{
	EVE_GPU_FONT_HEADER font_hdr;
	uint16_t width = 0;
	const char *ch = str;

	if (eve_ui_font_header(font_handle, &font_hdr) == 0)
	{
		while (*ch)
		{
			if (*ch < 128)
			{
				width += font_hdr.FontWidth[(int)*ch];
			}
			ch++;
		}
	}
	return width;
}

static uint32_t eve_ui_load_fontx(uint8_t first, const uint8_t *font_data, uint32_t font_size, uint8_t font_handle)
{
	const EVE_GPU_FONT_HEADER *font_hdr = (EVE_GPU_FONT_HEADER *)font_data;
	uint32_t hdr_addr;
	uint32_t end_addr = 0;
	int32_t bmp_offset;

	if ((font_data) && (font_size))
	{
		if (font_handle < 32)
		{
			// Enforce alignment.
			hdr_addr = malloc_ram_g(font_size);
			if (hdr_addr)
			{
				if (first == 0)
				{
					bmp_offset = hdr_addr + sizeof(EVE_GPU_FONT_HEADER)
								- (1 * font_hdr->FontLineStride * font_hdr->FontHeightInPixels);
				}
				else
				{
					bmp_offset = hdr_addr + sizeof(EVE_GPU_FONT_HEADER)
								- (1 * font_hdr->FontLineStride * font_hdr->FontHeightInPixels);
				}
				// Calculate the end of the area occupied by the font in RAM_G.
				end_addr = hdr_addr + font_size;
				// Store a pointer to the font header in the lookup array.
				font_pm_pointers[font_handle] = (EVE_GPU_FONT_HEADER *)font_data;

				eve_ui_arch_write_ram_from_pm((const uint8_t *)font_hdr, font_size, hdr_addr);
				// For EVE_CMD_SETFONT update the pointer to font graphics data.
				uint8_t font_gd[sizeof(uint32_t)];
				font_gd[0] = (bmp_offset >> 0) & 0xff;
				font_gd[1] = (bmp_offset >> 8) & 0xff;
				font_gd[2] = (bmp_offset >> 16) & 0xff;
				font_gd[3] = (bmp_offset >> 24) & 0xff;
				EVE_LIB_WriteDataToRAMG(font_gd, sizeof(uint32_t), 
						hdr_addr + offsetof(EVE_GPU_FONT_HEADER, PointerToFontGraphicsData));

				EVE_LIB_BeginCoProList();
				EVE_CMD_DLSTART();
				EVE_BEGIN(EVE_BEGIN_BITMAPS);
				EVE_BITMAP_HANDLE(font_handle);
				EVE_BITMAP_SIZE(EVE_FILTER_NEAREST, EVE_WRAP_BORDER, EVE_WRAP_BORDER,
						font_hdr->FontWidthInPixels,
						font_hdr->FontHeightInPixels);
				EVE_BITMAP_LAYOUT(font_hdr->FontBitmapFormat,
						font_hdr->FontLineStride, font_hdr->FontHeightInPixels);
				if (first == 0)
				{
					EVE_BITMAP_SOURCE(hdr_addr + sizeof(EVE_GPU_FONT_HEADER)
							- (1 * font_hdr->FontLineStride * font_hdr->FontHeightInPixels));
					EVE_CMD_SETFONT(font_handle, hdr_addr);
				}
				else
				{
					EVE_BITMAP_SOURCE(hdr_addr + sizeof(EVE_GPU_FONT_HEADER)
							- (first * font_hdr->FontLineStride * font_hdr->FontHeightInPixels));
					EVE_CMD_SETFONT2(font_handle, hdr_addr, first);
				}
				EVE_END();

				EVE_DISPLAY();
				EVE_CMD_SWAP();
				EVE_LIB_EndCoProList();
				EVE_LIB_AwaitCoProEmpty();
			}
		}
	}

	return end_addr;
}

uint32_t eve_ui_load_font(const uint8_t *font_data, uint32_t font_size, uint8_t font_handle)
{
	// Use CMD_SETFONT.
	return eve_ui_load_fontx(0, font_data, font_size, font_handle);
}

uint32_t eve_ui_load_font2(uint8_t first, const uint8_t *font_data, uint32_t font_size, uint8_t font_handle)
{
	// Use CMD_SETFONT2.
	return eve_ui_load_fontx(first, font_data, font_size, font_handle);
}
