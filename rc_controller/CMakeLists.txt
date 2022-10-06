# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/hnmsky/work/pi/pico/pico-sdk")
set(PICO_TINYUSB_PATH ${PICO_SDK_PATH}/lib/tinyusb)
set(FAMILY rp2040)
set(BOARD pico_sdk)
set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(rc_controller C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(${PICO_TINYUSB_PATH}/examples/device tinyusb_device_examples)
# Add executable. Default name is the project name, version 0.1

add_executable(rc_controller rc_controller.h rc_config.h)

target_sources(rc_controller PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/hid.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        ${CMAKE_CURRENT_LIST_DIR}/rc_controller.c
        )
#add_executable(rc_controller rc_controller.c )

pico_set_program_name(rc_controller "rc_controller")
pico_set_program_version(rc_controller "0.1")

pico_enable_stdio_uart(rc_controller 1)
pico_enable_stdio_usb(rc_controller 0)

target_include_directories(rc_controller PUBLIC
        ${CMAKE_CURRENT_LIST_DIR} )

# Add the standard library to the build
target_link_libraries(rc_controller  pico_stdlib)

# Add any user requested libraries
target_link_libraries(rc_controller
        hardware_pio
        tinyusb_device
        tinyusb_board
        )
pico_generate_pio_header(rc_controller ${CMAKE_CURRENT_LIST_DIR}/rc_controller.pio)
pico_add_extra_outputs(rc_controller)

