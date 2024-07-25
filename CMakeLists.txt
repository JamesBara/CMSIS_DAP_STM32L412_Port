cmake_minimum_required(VERSION 3.29)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME CMSIS_DAP_STM32L412_PORT)

# Include toolchain file
include("gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for C languages
enable_language(C)

# Core project settings
project(${CMAKE_PROJECT_NAME})

message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

add_subdirectory(STM32L4xx_USB_Device/STM32L4xx)
add_subdirectory(STM32L4xx_USB_Device)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    CMSIS-DAP/Firmware/Source/DAP.c
    CMSIS-DAP/Firmware/Source/DAP_vendor.c
    CMSIS-DAP/Firmware/Source/SW_DP.c
    src/bsp.c
    src/main.c
  )

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    CMSIS-DAP/Firmware/Include
    inc
)

target_compile_definitions(STM32L4xx INTERFACE
     $<$<CONFIG:Debug>:DEBUG>
    STM32L412
)

target_compile_definitions(STM32L4xx_USB_Device INTERFACE
    $<$<CONFIG:Debug>:DEBUG>
)


target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
     $<$<CONFIG:Debug>:DEBUG>
)


# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    STM32L4xx
    STM32L4xx_USB_Device
)

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        VERBATIM
        #COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
        #COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
        COMMAND ${CMAKE_OBJDUMP} -h ${PROJECT_NAME}.elf
        )