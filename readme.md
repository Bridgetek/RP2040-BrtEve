# RP2040-BrtEve (Arduino) 

This is a library to drive IDM2040-7A or IDM2040-43A with Arduino-IDE.

We also added several example projects using the library to this repo.

For more information about IDM2040-7A please visit us at [here](https://brtchip.com/product/idm2040-7a/)     
For more information about IDM2040-43A please visit us at [here](https://brtchip.com/product/idm2040-43a/)    


# To run this library and EVE examples with Arduino IDE

## Config Arduino IDE to support RP2040 based board.
   
### Configure the “additional board manager” of Arduino IDE with the URL by File->Preference dialogue:  

`https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`


![image](https://github.com/Bridgetek/RP2040-BrtEve/assets/13127756/0bae1f40-d68c-41ce-90e6-cfc14da33ce4)

### Install the RP2040 specific environment in the board manager.  Type “RP2040” to find the right package  

![image](https://github.com/user-attachments/assets/64271460-061f-4abe-9669-d7ac8b216ecb)



## Open Arduino IDE, select IDM2040-7A or IDM2040-43A board
   
![image](https://github.com/user-attachments/assets/eeb0f4bf-ecd7-4d89-84e9-594d6853c168)


## Install RP2040-BrtEve library from Arduino library.

### Find the RP2040-BrtEve library

![image](https://github.com/user-attachments/assets/2d6e708f-1589-4c74-bd05-b02163c8c014)

### Install the library

![image](https://github.com/user-attachments/assets/bdc19a2c-958f-4b37-8acf-0c3f13dd0ce8)

### The library will be instlled as following

![image](https://github.com/user-attachments/assets/da14dc87-6fcf-4f92-a320-7913447787fa)
  
## Select an example:

![image](https://github.com/user-attachments/assets/98a052c7-51a1-4322-b311-02119607cb05)


## Now you can build and run it!!

# Version
This version is v1.1.0

# Release log
v1.1.0 - Support both BridgeTek IDM2040-7A and BridgeTek IDM2040-43A

v1.0.0 - Support BridgeTek IDM2040-7A
