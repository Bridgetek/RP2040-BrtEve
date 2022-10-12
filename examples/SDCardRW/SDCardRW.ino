/*
  IDM2040 SD card read/write

  This example shows how to read an SD card content and display it on LCD of IDM2040
  
  The RP2040 connect to SD card via SPI1
  
*/

#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <IDM2040.h>

File myFile;
uint8_t dispText[100];
#define FILE_WRITE_NEW (O_READ | O_WRITE | O_CREAT)

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  Serial.print("Initializing EVE...");
  eve_ui_setup();

  Serial.print("Initializing SD card...");

  if (!SD.begin(PIN_SPI1_SS, SPI1)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE_NEW);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("Testing SDcard 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      myFile.read(dispText, sizeof(dispText));
      printf("%s", dispText);
    }

    // set null terminal to the string buffer
    int buffLen = myFile.size();
    printf("buffLen=%d\n", buffLen);
    buffLen = min(buffLen, sizeof(dispText));
    dispText[buffLen - 2] = 0;
    
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  eve_ui_splash((char*)dispText);
  delay(5000);
  // nothing happens after setup
}