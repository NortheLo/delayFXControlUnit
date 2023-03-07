cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
# initialize pico-sdk from submodule
# note: this must happen before project()
include(../pico-sdk/pico_sdk_init.cmake)

project(delay-control)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(delay-control src/delay-control.c)

# pull in common dependencies
target_link_libraries(delay-control pico_stdlib hardware_pwm hardware_i2c)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(delay-control)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
