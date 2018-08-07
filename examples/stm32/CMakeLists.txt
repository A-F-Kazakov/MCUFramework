cmake_minimum_required(VERSION 2.8)
project(ARM CXX ASM)

include(${MCU_ROOT}/cmake/mcu.cmake)

setupDevice("STM32F407VG")

include(${MCU_ROOT}/cmake/toolchain-arm-gcc.cmake)

set(SOURCE_FILES ${SOURCE_FILES} ArmTest.cpp)

set(NAME "${PROJECT_NAME}_Example")

add_executable(${NAME} ${SOURCE_FILES})

target_link_libraries(${NAME} MCU)
#target_include_directories(${NAME} INTERFACE ${MCU_INCLUDE})
#target_compile_definitions(MCU PUBLIC ${MCU_COMPILE_FLAGS})

#build_ARM(${NAME})
#UPLOAD_HEX(${NAME})