/*
  Game: Space Invader
*/

#include "IDM2040.h"
#include "Invaders.h"

// the setup routine runs once when you press reset:
void setup()
{
    // initialize serial communication at 9600 bits per second:
    Serial.begin(115200);

    /* Start example code. Includes EVE setup. */
    invader_setup();
}

// the loop routine runs over and over again forever:
void loop()
{
    for (;;)
    {
        invader_loop();
    }
    delay(1); // delay in between reads for stability
}
