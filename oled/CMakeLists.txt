cmake_minimum_required(VERSION 3.12)

# for clangd lsp to resolve properlyh
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# or through -DPICO_BOARD=pico2_w when initializing build folder
set(PICO_BOARD pico2_w)

# load sdk's cmake
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
pico_sdk_init()


project(picoproj)

add_executable(picoproj
    main.c
    src/screen.c
    src/screen.h
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(picoproj pico_stdlib pico_cyw43_arch_none hardware_spi hardware_pwm)

# create map/bin/hex/uf2 file in addition to ELF
pico_add_extra_outputs(picoproj)

# Enable usb output, disable uart (serial port) output
pico_enable_stdio_usb(picoproj 1)
pico_enable_stdio_uart(picoproj 0)
