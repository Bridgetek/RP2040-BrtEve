# RP2040-BrtEve

This is a library to drive IDM2040-7A with Arduino-IDE.

We also added some EVE example to this repo.

For more information about IDM2040-7A please visit us at https://brtchip.com/ic-module/product/idm2040-7a/

# To run this library and EVE examples with Arduino IDE

1. Goto Arduino IDE download folder, clone the Arduino-Pico repo:

    mkdir -p ~/Arduino/hardware/pico
    git clone https://github.com/earlephilhower/arduino-pico.git ~/Arduino/hardware/pico/rp2040
    cd ~/Arduino/hardware/pico/rp2040
    git submodule add --force https://github.com/Bridgetek/RP2040-BrtEve libraries/RP2040-BrtEve
    git submodule update --init
    cd pico-sdk
    git submodule update --init
    cd ../tools
    python3 ./get.py # or: py -3 ./get.py
    
2. Open Arduino IDE, select IDM2040-7A board
   
   ![image](https://user-images.githubusercontent.com/48436368/209924942-242e1a02-795b-4385-a667-da5c7816230d.png)
  
3. Select an EVE example and run:

   ![image](https://user-images.githubusercontent.com/48436368/209924823-cc57825d-e93e-4412-b88f-f21393bb25dd.png)

    
    
    
    
