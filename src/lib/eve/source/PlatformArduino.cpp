/**
* @file PlatformArduino.cpp
* @brief This source file will contain all required Arduino Platform functions
*
* @author Tuan Nguyen <tuan.nguyen@brtchip.com>
*
* @date 2022
*/

#include <stdio.h>
#include <Arduino.h>
#include <print.h>

/* Only compile for ARDUINO platforms */
#ifdef ARDUINO_ARCH_RP2040

/**
 * @brief Enable the printf for BridgeTek IDM2040
 * 
 */
extern "C" int EVE_Printf(char *format, ...){
	#define BUFFER_SIZE 1024
	char buffer[BUFFER_SIZE];    
   
    va_list vl;
	va_start(vl, format);
	memset(buffer, 0, BUFFER_SIZE);
	int ret = vsnprintf(buffer, BUFFER_SIZE, format, vl);
	Serial.print(buffer);
	va_end(vl);
	return ret;
}

#endif // ARDUINO_ARCH_RP2040


