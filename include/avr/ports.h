
#ifndef AVR_PORTS_H
#define AVR_PORTS_H

#ifndef IOPORTS_H
#error "Do not include this file directly. Use ioports.h instead."
#endif

namespace velo
{
	namespace embedded
	{
		#define MAKE_PORT(portName, ddrName, pinName, className, ID)   \
      class className{                                             	\
      public:                                                      	\
         typedef uint8_t dataT;                                    	\
      private:                                                      	\
         static volatile dataT &data() { return portName; }        	\
         static volatile dataT &dir() { return ddrName; }         	\
         static volatile dataT &pin() { return pinName; }         	\
      public:                                                      	\
         static void write(dataT value) { data() = value; }         	\
         static void clearAndSet(dataT clearMask, dataT value)      	\
         { data() = (data() & ~clearMask) | value; }               	\
         static dataT read() { return data(); }                     	\
         static void writeDir(dataT value) { dir() = value; }      	\
         static dataT readDir() { return dir(); }                  	\
         static void set(dataT value) { data() |= value; }         	\
         static void clear(dataT value) { data() &= ~value; }      	\
         static void toggle(dataT value) { data() ^= value; }      	\
         static void setDir(dataT value) { dir() |= value; }      	\
         static void clearDir(dataT value) { dir() &= ~value; }   	\
         static void toggleDir(dataT value) { dir() ^= value; }   	\
         static dataT readPin() { return pin(); }                  	\
         enum{Id = ID};                                             	\
         enum{width = sizeof(dataT) * 8};                           	\
      };

		#ifdef PORTA

		MAKE_PORT(PORTA, DDRA, PINA, PortA, 'A')

		#endif

		#ifdef PORTB

		MAKE_PORT(PORTB, DDRB, PINB, PortB, 'B')

		#endif

		#ifdef PORTC

		MAKE_PORT(PORTC, DDRC, PINC, PortC, 'C')

		#endif

		#ifdef PORTD

		MAKE_PORT(PORTD, DDRD, PIND, PortD, 'D')

		#endif

		#ifdef PORTE
		MAKE_PORT(PORTE, DDRE, PINE, PortE, 'E')
		#endif

		#ifdef PORTF
		MAKE_PORT(PORTF, DDRF, PINF, PortF, 'F')
		#endif

		#ifdef PORTG
		MAKE_PORT(PORTG, DDRG, PING, PortG, 'G')
		#endif
	}
}

#endif // AVR_PORTS_H