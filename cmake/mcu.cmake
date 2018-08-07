# This is the MCU library configuration module

macro(setupdevice DEVICE)
	if(${DEVICE} STREQUAL "STM32F407VG")

		set(DEVICE_FAMILY "cortex-m4")
		set(DEVICE_FLASH_SIZE "1M")
		set(DEVICE_RAM_SIZE "192K")
		set(DEVICE_STACK_ADDRESS "0x20010000")
		set(DEVICE_FLASH_ORIGIN "0x08000000")
		set(DEVICE_RAM_ORIGIN "0x20000000")

		set(DEVICE_DEFINITION "STM32F40_41xxx")
		set(DEVICE_HSE_DEFINITION "HSE_VALUE=8000000u")

		if(NOT MCU_ROOT)
			message(ERROR "MCU_ROOT should be specified")
		endif()

		if(DEVICE_FAMILY STREQUAL "cortex-m4")
			set(MCU_COMPILER_OPTIONS "${MCU_COMPILER_OPTIONS} -mcpu=cortex-m4 -march=armv7e-m")
		elseif(DEVICE_FAMILY STREQUAL "cortex-m3")
			set(MCU_COMPILER_OPTIONS "${MCU_COMPILER_OPTIONS} -mcpu=cortex-m3 -march=armv7-m -msoft-float")
		elseif(DEVICE_FAMILY STREQUAL "cortex-a9")
			set(MCU_COMPILER_OPTIONS "${MCU_COMPILER_OPTIONS} -mcpu=cortex-a9 -march=armv7-a -mthumb -mfloat-abi=hard -mfpu=neon")
		else()
			message(ERROR "Device family ${DEVICE_FAMILY} not recognized")
		endif()

		message(STATUS "Device: ${DEVICE}")
		message(STATUS "- family: ${DEVICE_FAMILY}")
		message(STATUS "- flash memory size: ${DEVICE_FLASH_SIZE}")
		message(STATUS "- RAM memory size: ${DEVICE_RAM_SIZE}")

		set(MCU_NATIVE_DIR "${MCU_ROOT}/include/stm32f4xx")

		set(MCU_CMSIS_ROOT_DIR "${MCU_NATIVE_DIR}/cmsis")
		set(MCU_CMSIS_INCLUDE_DIR "${MCU_NATIVE_DIR}/cmsis/include")
		set(MCU_CMSIS_STARTUP_DIR "${MCU_NATIVE_DIR}/cmsis/startups")

		set(CMSIS_CORE_HEADERS
			 ${MCU_CMSIS_INCLUDE_DIR}/core_cm4.h
			 ${MCU_CMSIS_INCLUDE_DIR}/core_cmFunc.h
			 ${MCU_CMSIS_INCLUDE_DIR}/core_cmInstr.h
			 ${MCU_CMSIS_INCLUDE_DIR}/core_cmSimd.h)

		set(CMSIS_SYSTEM_FILES
			 ${MCU_CMSIS_INCLUDE_DIR}/stm32f4xx.h
			 ${MCU_CMSIS_STARTUP_DIR}/startup_stm32f4xx.s
			 ${MCU_CMSIS_INCLUDE_DIR}/system_stm32f4xx.h
			 ${MCU_CMSIS_ROOT_DIR}/system_stm32f4xx.c)

		set(CMSIS_FILES ${CMSIS_CORE_HEADERS} ${CMSIS_SYSTEM_FILES})

		set(LINKER_FILE "${MCU_CMSIS_ROOT_DIR}/stm32_flash.ld.in")

		configure_file(${LINKER_FILE} ${CMAKE_CURRENT_BINARY_DIR}/stm32_flash.ld)
		set(LINKER "-T${CMAKE_CURRENT_BINARY_DIR}/stm32_flash.ld")

		set(SOURCE_FILES ${CMSIS_FILES} ${MCU_NATIVE_DIR}/clock.h ${MCU_NATIVE_DIR}/native_ports.h)

	else()
		message(FATAL_ERROR "Supported devices to build is: STM32F407VG")
	endif()

	set(SOURCE_FILES ${SOURCE_FILES}
		 ${MCU_ROOT}/include/ioRegisters.h
		 ${MCU_ROOT}/include/iopin.h
		 ${MCU_ROOT}/include/ioports.h)

	set(MCU_COMPILE_FLAGS "-Wall -ffunction-sections -fno-builtin -fdata-sections -fno-exceptions")

	set(MCU_COMPILE_FLAGS "-D${DEVICE_DEFINITION} -D${DEVICE_HSE_DEFINITION} ${MCU_COMPILER_OPTIONS} ${LINKER} ${MCU_COMPILE_FLAGS}")

	set(CMAKE_CXX_FLAGS ${MCU_COMPILE_FLAGS})
	set(CMAKE_C_FLAGS   ${MCU_COMPILE_FLAGS})

	set(MCU_INCLUDE ${MCU_ROOT}/include ${MCU_NATIVE_DIR} ${MCU_CMSIS_INCLUDE_DIR})

	add_library(MCU INTERFACE)
	target_include_directories(MCU INTERFACE ${MCU_INCLUDE})

endmacro()