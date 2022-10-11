/**
* @file PlatformArduino.h
* @brief This header file will contain all required Arduino platform functions
*
* @author Tuan Nguyen <tuan.nguyen@brtchip.com>
*
* @date 2022
*/

#ifndef PLATFORM_ARDUINO_H
#define	PLATFORM_ARDUINO_H
#include <stdio.h>

#define printf EVE_Printf
int EVE_Printf(char *format, ...);

#endif	/* PLATFORM_ARDUINO_H */
