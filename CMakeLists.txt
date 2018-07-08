cmake_minimum_required(VERSION 2.8)
project(MCU C CXX ASM)

set(MCU_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

#=======================================================================================================================
# Library
#=======================================================================================================================

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
									${MCU_INCLUDE_DIR}
									${MCU_INCLUDE_DIR}/avr
									${MCU_INCLUDE_DIR}/stm32f4xx)

target_link_libraries(${PROJECT_NAME} INTERFACE Generic)

#=======================================================================================================================
# Examples
#=======================================================================================================================

#add_subdirectory(avr)
add_subdirectory(test/stm32)