# delayFXControlUnit
## Overview
This is the source code for the Raspberry Pico mcu to controll my delay pedal.
The purpose of this code is to read the pot and switch values and produce the desired modulation for the PT-2399s delay time pin.

## Build
1. Clone this repo with ``git clone``
2. Create your build directory and run Cmake to check, if you have correctly installed the pico SDK with ``mkdir build && cd build`` and ``cmake ..``
3. Build the projekt with ``make -j $(nproc --all)
4. Copy the binary into the Flash of the Raspberry Pico
