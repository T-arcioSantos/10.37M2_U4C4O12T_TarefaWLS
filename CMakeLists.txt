# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================


cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)
project(10.37M2_U4C4O12T_TarefaWLS C CXX ASM)
pico_sdk_init()
add_executable(10.37M2_U4C4O12T_TarefaWLS 10.37M2_U4C4O12T_TarefaWLS.c )
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
pico_generate_pio_header(10.37M2_U4C4O12T_TarefaWLS ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)
target_sources(10.37M2_U4C4O12T_TarefaWLS PRIVATE 10.37M2_U4C4O12T_TarefaWLS.c )
pico_set_program_name(10.37M2_U4C4O12T_TarefaWLS "10.37M2_U4C4O12T_TarefaWLS")
target_link_libraries(10.37M2_U4C4O12T_TarefaWLS pico_stdlib hardware_pio)
pico_add_extra_outputs(10.37M2_U4C4O12T_TarefaWLS)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(10.37M2_U4C4O12T_TarefaWLS 1)
pico_enable_stdio_usb(10.37M2_U4C4O12T_TarefaWLS 1)

