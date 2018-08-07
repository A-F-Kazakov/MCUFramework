cmake_minimum_required(VERSION 2.8)
project(MCU C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(${PROJECT_NAME}_BUILD_EXAMPLES "Set to ON to build examples" ON)

set(MCU_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(examples/stm32)