
#ifndef IOPIN_H
#define IOPIN_H

#include <stdint.h>
#include <ioports.h>

namespace velo
{
	namespace embedded
	{
		template<typename PORT, uint8_t PIN>
		class Pin
		{
			public:
				typedef PORT Port;
				enum
				{
					number = PIN
				};

				static void set()
				{
					PORT::set(1 << PIN);
				}

				static void set(uint8_t val)
				{
					if(val)
						set();
					else
						clear();
				}

				static void setDir(uint8_t val)
				{
					if(val)
						setDirWrite();
					else
						setDirRead();
				}

				static void clear()
				{
					PORT::clear(1 << PIN);
				}

				static void toggle()
				{
					PORT::toggle(1 << PIN);
				}

				static void setDirRead()
				{
					PORT::clearDir(1 << PIN);
				}

				static void setDirWrite()
				{
					PORT::setDir(1 << PIN);
				}

				static uint8_t isSet()
				{
					return PORT::readPin() & (uint8_t) (1 << PIN);
				}

				static void waiteForSet()
				{
					while(isSet() == 0);
				}

				static void WaiteForClear()
				{
					while(isSet());
				}
		};

		//Short pin definations
		#ifdef PORTA
		typedef Pin<PortA, 0> Pa0;
		typedef Pin<PortA, 1> Pa1;
		typedef Pin<PortA, 2> Pa2;
		typedef Pin<PortA, 3> Pa3;
		typedef Pin<PortA, 4> Pa4;
		typedef Pin<PortA, 5> Pa5;
		typedef Pin<PortA, 6> Pa6;
		typedef Pin<PortA, 7> Pa7;
		#endif

		#ifdef PORTB
		typedef Pin<PortB, 0> Pb0;
		typedef Pin<PortB, 1> Pb1;
		typedef Pin<PortB, 2> Pb2;
		typedef Pin<PortB, 3> Pb3;
		typedef Pin<PortB, 4> Pb4;
		typedef Pin<PortB, 5> Pb5;
		typedef Pin<PortB, 6> Pb6;
		typedef Pin<PortB, 7> Pb7;
		#endif

		#ifdef PORTC
		typedef Pin<PortC, 0> Pc0;
		typedef Pin<PortC, 1> Pc1;
		typedef Pin<PortC, 2> Pc2;
		typedef Pin<PortC, 3> Pc3;
		typedef Pin<PortC, 4> Pc4;
		typedef Pin<PortC, 5> Pc5;
		typedef Pin<PortC, 6> Pc6;
		typedef Pin<PortC, 7> Pc7;
		#endif

		#ifdef PORTD
		typedef Pin<PortD, 0> Pd0;
		typedef Pin<PortD, 1> Pd1;
		typedef Pin<PortD, 2> Pd2;
		typedef Pin<PortD, 3> Pd3;
		typedef Pin<PortD, 4> Pd4;
		typedef Pin<PortD, 5> Pd5;
		typedef Pin<PortD, 6> Pd6;
		typedef Pin<PortD, 7> Pd7;
		#endif

		#ifdef PORTE
		typedef Pin<PortE, 0> Pe0;
		typedef Pin<PortE, 1> Pe1;
		typedef Pin<PortE, 2> Pe2;
		typedef Pin<PortE, 3> Pe3;
		typedef Pin<PortE, 4> Pe4;
		typedef Pin<PortE, 5> Pe5;
		typedef Pin<PortE, 6> Pe6;
		typedef Pin<PortE, 7> Pe7;
		#endif

		#ifdef PORTF
		typedef Pin<PortF, 0> Pf0;
		typedef Pin<PortF, 1> Pf1;
		typedef Pin<PortF, 2> Pf2;
		typedef Pin<PortF, 3> Pf3;
		typedef Pin<PortF, 4> Pf4;
		typedef Pin<PortF, 5> Pf5;
		typedef Pin<PortF, 6> Pf6;
		typedef Pin<PortF, 7> Pf7;
		#endif

		#ifdef PORTG
		typedef Pin<PortG, 0> Pg0;
		typedef Pin<PortG, 1> Pg1;
		typedef Pin<PortG, 2> Pg2;
		typedef Pin<PortG, 3> Pg3;
		typedef Pin<PortG, 4> Pg4;
		typedef Pin<PortG, 5> Pg5;
		typedef Pin<PortG, 6> Pg6;
		typedef Pin<PortG, 7> Pg7;
		#endif

		#ifdef PORTH
		typedef Pin<PortH, 0> Ph0;
		typedef Pin<PortH, 1> Ph1;
		typedef Pin<PortH, 2> Ph2;
		typedef Pin<PortH, 3> Ph3;
		typedef Pin<PortH, 4> Ph4;
		typedef Pin<PortH, 5> Ph5;
		typedef Pin<PortH, 6> Ph6;
		typedef Pin<PortH, 7> Ph7;
		#endif

		#ifdef PORTJ
		typedef Pin<PortJ, 0> Pj0;
		typedef Pin<PortJ, 1> Pj1;
		typedef Pin<PortJ, 2> Pj2;
		typedef Pin<PortJ, 3> Pj3;
		typedef Pin<PortJ, 4> Pj4;
		typedef Pin<PortJ, 5> Pj5;
		typedef Pin<PortJ, 6> Pj6;
		typedef Pin<PortJ, 7> Pj7;
		#endif

		#ifdef PORTK
		typedef Pin<PortK, 0> Pk0;
		typedef Pin<PortK, 1> Pk1;
		typedef Pin<PortK, 2> Pk2;
		typedef Pin<PortK, 3> Pk3;
		typedef Pin<PortK, 4> Pk4;
		typedef Pin<PortK, 5> Pk5;
		typedef Pin<PortK, 6> Pk6;
		typedef Pin<PortK, 7> Pk7;
		#endif

		#ifdef PORTQ
		typedef Pin<PortQ, 0> Pq0;
		typedef Pin<PortQ, 1> Pq1;
		typedef Pin<PortQ, 2> Pq2;
		typedef Pin<PortQ, 3> Pq3;
		typedef Pin<PortQ, 4> Pq4;
		typedef Pin<PortQ, 5> Pq5;
		typedef Pin<PortQ, 6> Pq6;
		typedef Pin<PortQ, 7> Pq7;
		#endif

		#ifdef PORTR
		typedef Pin<PortR, 0> Pr0;
		typedef Pin<PortR, 1> Pr1;
		typedef Pin<PortR, 2> Pr2;
		typedef Pin<PortR, 3> Pr3;
		typedef Pin<PortR, 4> Pr4;
		typedef Pin<PortR, 5> Pr5;
		typedef Pin<PortR, 6> Pr6;
		typedef Pin<PortR, 7> Pr7;
		#endif
	}
}

#endif // IOPIN_H
