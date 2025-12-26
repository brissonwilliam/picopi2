cmake_minimum_required(VERSION 3.12)

# for clangd lsp to resolve properlyh
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# or through -DPICO_BOARD=pico2_w when initializing build folder
set(PICO_BOARD pico2_w)

# load sdk's cmake
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(blink)

pico_sdk_init()

add_executable(blink
    main.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(blink pico_stdlib pico_cyw43_arch_none)

# create map/bin/hex/uf2 file in addition to ELF
pico_add_extra_outputs(blink)

# Enable usb output, disable uart (serial port) output
pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)
