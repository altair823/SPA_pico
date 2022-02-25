# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/Users/altair823/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(SPA_pico C CXX ASM)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(SPA_pico SPA_pico.cpp Maze.h SPA/ASPQ.h SPA/DIK.h SPA/ASBQ.h structure/BucketQueue.h SPA/SPA.h structure/PriorityQueue.h)

pico_set_program_name(SPA_pico "SPA_pico")
pico_set_program_version(SPA_pico "0.1")

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Add the standard library to the build
target_link_libraries(SPA_pico pico_stdlib)

# Add any user requested libraries
target_link_libraries(SPA_pico
        hardware_spi
        hardware_pio
        hardware_timer
        pico_time
        hardware_i2c
        )

pico_add_extra_outputs(SPA_pico)

