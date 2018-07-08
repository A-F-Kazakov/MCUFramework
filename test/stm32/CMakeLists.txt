cmake_minimum_required(VERSION 2.8)
project(ARM CXX ASM)

set(DEVICE "STM32F407VG")

option(BUILD_EXAMPLES "Build exmaples" ON)
option(UPLOAD "Upload binary" OFF)

#=======================================================================================================================
# include configuration files
#=======================================================================================================================

include(../../../../Utils/cmake/velo_mcu.cmake)
include(../../../../Utils/cmake/toolchain-arm-gcc.cmake)

#=======================================================================================================================
# build executable
#=======================================================================================================================

set(SOURCES_LIST ArmTest.cpp
	 ${MCU_INCLUDE_DIR}/stm32f4xx/clock.h
	 ${MCU_INCLUDE_DIR}/stm32f4xx/native_ports.h
	 ${MCU_INCLUDE_DIR}/ioRegisters.h
	 ${MCU_INCLUDE_DIR}/iopin.h
	 ${MCU_INCLUDE_DIR}/ioports.h
	 ${MCU_INCLUDE_DIR}/Button.h
	 ${MCU_INCLUDE_DIR}/Spi.h)

#set(MY_COMPILE_FLAGS "-Wall -fno-common -ffunction-sections -fno-builtin -fdata-sections -fno-exceptions --specs=nosys.specs")
set(MY_COMPILE_FLAGS "-Wall -ffunction-sections -fno-builtin -fdata-sections -fno-exceptions")

set(CMAKE_CXX_FLAGS "-D${DEVICE_DEFINITION} -D${DEVICE_HSE_DEFINITION} ${MCU_COMPILER_OPTIONS} ${LINKER} ${MY_COMPILE_FLAGS}")
set(CMAKE_C_FLAGS "-D${DEVICE_DEFINITION} -D${DEVICE_HSE_DEFINITION} ${MCU_COMPILER_OPTIONS} ${LINKER} ${MY_COMPILE_FLAGS}")

set(NAME "${PROJECT_NAME}_Example")
add_executable(${NAME} ${CMSIS_FILES} ${SOURCES_LIST})

#set_target_properties(${NAME} PROPERTIES COMPILE_FLAGS ${MCU_COMPILER_OPTIONS})
target_include_directories(${NAME} PUBLIC ${CMSIS_CORE_DIR} ${CMSIS_SYSTEM_DIR})
target_link_libraries(${NAME} MCU)

build_ARM(${NAME})
#UPLOAD_HEX(${NAME})