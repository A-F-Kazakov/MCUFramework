/**
 * @author	K
 * @date		25-05-2017.
 */

#ifndef VELO_ARMPORTS_H
#define VELO_ARMPORTS_H

#ifndef IOPORTS_H
#error "Do not include this file directly. Use ioports.h instead."
#endif

#include <stm32f4xx.h>
#include <ioRegisters.h>
#include <clock.h>

namespace velo
{
	namespace mcu
   {
		namespace io
		{
			enum class Configuration
			{
					Analog = 3, In = 0, Out = 1, AltFunc = 2
			};

			enum class PullMode
			{
					NoPullUp = 0, PullUp = 1, PullDown = 2
			};

			enum class DriverType
			{
					PushPull = 0, OpenDrain = 1
			};

			enum class Speed
			{
					Slow = 0, Medium = 1, Fast = 2, Fastest = 3
			};

			namespace internal
			{
				class NativePort
				{
					public:
						enum{ width = 16 };
						typedef uint16_t DataT;

						static inline unsigned unpackConfig2bits(uint32_t mask, uint32_t value, uint32_t configuration)
						{
							mask = (mask & 0xff00) << 8 | (mask & 0x00ff);
							mask = (mask & 0x00f000f0) << 4 | (mask & 0x000f000f);
							mask = (mask & 0x0C0C0C0C) << 2 | (mask & 0x03030303);
							mask = (mask & 0x22222222) << 1 | (mask & 0x11111111);
							return (value & ~(mask * 0x03)) | mask * configuration;
						}

						static inline unsigned unpackConfig4Bit(uint32_t mask, uint32_t value, uint32_t configuration)
						{
							mask = (mask & 0xf0) << 12 | (mask & 0x0f);
							mask = (mask & 0x000C000C) << 6 | (mask & 0x00030003);
							mask = (mask & 0x02020202) << 3 | (mask & 0x01010101);
							return (value & ~(mask * 0x0f)) | mask * configuration;
						}
				};

				template<typename GpioRegs, typename ClkEnReg, int ID>
				class PortImplementation : public NativePort
				{
					public:
						static DataT read()
						{
							return GpioRegs()->ODR;
						}

						static void write(DataT value)
						{
							GpioRegs()->ODR = value;
						}

						static void clearAndSet(DataT clearMask, DataT value)
						{
							clear(clearMask);
							set(value);
						}

						static void set(DataT value)
						{
							GpioRegs()->BSRRL = (uint16_t) value;
						}

						static void clear(DataT value)
						{
							GpioRegs()->BSRRH = (uint16_t) value;
						}

						static void toggle(DataT value)
						{
							clearAndSet(value, (~GpioRegs()->ODR) & value);
						}

						static DataT readPin()
						{
							return GpioRegs()->IDR;
						}

						// constant interface

						template<DataT clearMask, DataT value> static void clearAndSet()
						{
							clear(clearMask);
							set(value);
						}

						template<DataT value> static void toggle()
						{
							clearAndSet(value, (~GpioRegs()->ODR) & value);
						}

						template<DataT value> static void set()
						{
							GpioRegs()->BSRRL = (uint16_t) value;
						}

						template<DataT value> static void clear()
						{
							GpioRegs()->BSRRH = (uint16_t) value;
						}

						/**
						 *  port configuration section
						 */

						static void configure(DataT mask, Configuration configuration)
						{
							GpioRegs()->MODER = unpackConfig2bits(mask, GpioRegs()->MODER, static_cast<DataT>(configuration));
						}

						template<DataT mask, Configuration configuration> static void configure()
						{
							GpioRegs()->MODER = unpackConfig2bits(mask, GpioRegs()->MODER, static_cast<DataT>(configuration));
						}

						static void configure(DataT mask, Speed speed)
						{
							GpioRegs()->OSPEEDR = unpackConfig2bits(mask, GpioRegs()->OSPEEDR, static_cast<DataT>(speed));
						}

						static void configure(DataT mask, PullMode pull)
						{
							GpioRegs()->PUPDR = unpackConfig2bits(mask, GpioRegs()->PUPDR, static_cast<DataT>(pull));
						}

						static void configure(DataT mask, DriverType driver)
						{
							GpioRegs()->OTYPER = unpackConfig2bits(mask, GpioRegs()->OTYPER, static_cast<DataT>(driver));
						}

						static void altFuncNumber(DataT mask, uint8_t number)
						{
							GpioRegs()->AFR[0] = unpackConfig4Bit(mask & 0xff, GpioRegs()->AFR[0], number);
							GpioRegs()->AFR[1] = unpackConfig4Bit((mask >> 8) & 0xff, GpioRegs()->AFR[1], number);
						}

						static void enable()
						{
							ClkEnReg::enable();
						}

						static void disable()
						{
							ClkEnReg::eisable();
						}

						enum{ id = ID };
				};
			}

			class Voidd;

#define MAKE_PORT(GPIO_REGS, ClkEnReg, className, ID)                   \
         IO_STRUCT_WRAPPER(GPIO_REGS, className ## Regs, GPIO_TypeDef);	\
        typedef internal::PortImplementation<                           \
            className ## Regs, ClkEnReg, ID> className;

			MAKE_PORT(GPIOA, clock::GpioAClock, PortA, 'A')

			MAKE_PORT(GPIOB, clock::GpioBClock, PortB, 'B')

			MAKE_PORT(GPIOC, clock::GpioCClock, PortC, 'C')

			MAKE_PORT(GPIOD, clock::GpioDClock, PortD, 'D')

			MAKE_PORT(GPIOE, clock::GpioEClock, PortE, 'E')

			MAKE_PORT(GPIOF, clock::GpioFClock, PortF, 'F')

			MAKE_PORT(GPIOG, clock::GpioGClock, PortG, 'G')

			MAKE_PORT(GPIOH, clock::GpioHClock, PortH, 'H')

			MAKE_PORT(GPIOI, clock::GpioIClock, PortI, 'I')

			MAKE_PORT(GPIOJ, clock::GpioJClock, PortJ, 'J')

			MAKE_PORT(GPIOK, clock::GpioKClock, PortK, 'K')

			#define MCUCPP_HAS_PORTA
			#define MCUCPP_HAS_PORTB
			#define MCUCPP_HAS_PORTC
			#define MCUCPP_HAS_PORTD
			#define MCUCPP_HAS_PORTE
			#define MCUCPP_HAS_PORTF
			#define MCUCPP_HAS_PORTG
			#define MCUCPP_HAS_PORTH
			#define MCUCPP_HAS_PORTI
			#define MCUCPP_HAS_PORTJ
			#define MCUCPP_HAS_PORTK
		}
   }
}

#endif //VELO_ARMPORTS_H
