/*
  Helloworld

  Print a simple text on an LCD
*/

#include "IDM2040.h"

// the setup routine runs once when you press reset:
void setup()
{
    // initialize serial communication at 9600 bits per second:
    Serial.begin(115200);

    // Initialise the display.
    eve_ui_setup();
}

// the loop routine runs over and over again forever:
void loop()
{
    eve_ui_splash("Hello world");
    delay(1); // delay in between reads for stability
}