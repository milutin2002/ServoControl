# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
SET(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/port/FreeRTOS-Kernel" CACHE STRING "Local Config")
include_directories("${FREERTOS_CONFIG_FILE_DIRECTORY}") 
include(FreeRTOS_Kernel_import.cmake)

project(ServoCode C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(ServoCode ServoCode.c servo.c wifi/wifi.c mqtt/mqtt.c)

pico_set_program_name(ServoCode "ServoCode")
pico_set_program_version(ServoCode "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(ServoCode 1)
pico_enable_stdio_usb(ServoCode 1)

# Add the standard library to the build
target_link_libraries(ServoCode
        pico_stdlib pico_stdlib FreeRTOS-Kernel-Heap4
        pico_lwip_mqtt pico_cyw43_arch_lwip_sys_freertos)

# Add the standard include files to the build
target_include_directories(ServoCode PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(ServoCode 
        hardware_pwm
        )
target_compile_definitions(ServoCode PRIVATE LWIP_TIMEVAL_PRIVATE=0 LWIP_POSIX_SOCKETS_IO_NAMES=0 PICO_CYW43_ARCH_TREADSAFE_BACKGROUND=1)

if(DEFINED WIFI_SSID)
    target_compile_definitions(ServoCode PRIVATE WIFI_SSID="${WIFI_SSID}")
endif()
if(DEFINED WIFI_PASS)
    target_compile_definitions(ServoCode PRIVATE WIFI_PASS="${WIFI_PASS}")
endif()

pico_add_extra_outputs(ServoCode)

