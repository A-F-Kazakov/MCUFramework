/**
 * @author	K
 * @date		09-06-2017.
 */

#ifndef EMBEDDED_IOPIN_H
#define EMBEDDED_IOPIN_H

namespace velo
{
	namespace mcu
	{
		namespace io
		{
			template<typename PORT, uint8_t PIN, typename CONFIG_PORT = PORT>
			class Pin
			{
					static_assert(PIN < PORT::width, "pin not fit");
				public:
					typedef PORT Port;
//		typedef CONFIG_PORT ConfigPort;
//
//		typedef typename ConfigPort::Speed Speed;
//		typedef typename ConfigPort::PullMode PullMode;
//		typedef typename ConfigPort::DriverType DriverType;
//		typedef typename ConfigPort::Configuration Configuration;

					static const uint8_t number   = PIN;
					static const bool    inverted = false;

					static void set()
					{
						PORT::template set<1 << PIN>();
					}

					static void clear()
					{
						PORT::template clear<1 << PIN>();
					}

					static void toggle()
					{
						PORT::template toggle<1 << PIN>();
					}

//					static void setDir(uint8_t val);

					static void setDirRead()
					{
						PORT::configure(1 << PIN, Configuration::In);
					}

					static void setDirWrite()
					{
						PORT::configure(1 << PIN, Configuration::Out);
					}

					static void configure(Configuration configuration)
					{
						PORT::configure(1 << PIN, configuration);
					}

					static void configure(DriverType driverType)
					{
						PORT::configure(1 << PIN, driverType);
					}

					static void configure(PullMode pullMode)
					{
						PORT::configure(1 << PIN, pullMode);
					}

					static void configure(Speed speed)
					{
						PORT::configure(1 << PIN, speed);
					}

					static void altFuncNumber(uint8_t funcNumber)
					{
						PORT::altFuncNumber(1 << PIN, funcNumber);
					}

//					template<Configuration configuration> static void SetConfiguration()
//					{
//						CONFIG_PORT::template setConfiguration<1 << PIN, configuration>();
//					}

					static bool isSet()
					{
						return (PORT::readPin() & (typename PORT::DataT)(1u << PIN)) != 0;
					}

					static void waiteForSet()
					{
						while(isSet() == 0);
					}

					static void waiteForClear()
					{
						while(isSet());
					}
			};

#define DECLARE_PORT_PINS(PORT_TYPE_NAME, PIN_NAME_PREFIX)		\
	typedef Pin<PORT_TYPE_NAME, 0> PIN_NAME_PREFIX ## 0;			\
	typedef Pin<PORT_TYPE_NAME, 1> PIN_NAME_PREFIX ## 1;			\
	typedef Pin<PORT_TYPE_NAME, 2> PIN_NAME_PREFIX ## 2;			\
	typedef Pin<PORT_TYPE_NAME, 3> PIN_NAME_PREFIX ## 3;			\
	typedef Pin<PORT_TYPE_NAME, 4> PIN_NAME_PREFIX ## 4;			\
	typedef Pin<PORT_TYPE_NAME, 5> PIN_NAME_PREFIX ## 5;			\
	typedef Pin<PORT_TYPE_NAME, 6> PIN_NAME_PREFIX ## 6;			\
	typedef Pin<PORT_TYPE_NAME, 7> PIN_NAME_PREFIX ## 7;			\
	typedef Pin<PORT_TYPE_NAME, 8> PIN_NAME_PREFIX ## 8;			\
	typedef Pin<PORT_TYPE_NAME, 9> PIN_NAME_PREFIX ## 9;			\
	typedef Pin<PORT_TYPE_NAME, 10> PIN_NAME_PREFIX ## 10;		\
	typedef Pin<PORT_TYPE_NAME, 11> PIN_NAME_PREFIX ## 11;		\
	typedef Pin<PORT_TYPE_NAME, 12> PIN_NAME_PREFIX ## 12;		\
	typedef Pin<PORT_TYPE_NAME, 13> PIN_NAME_PREFIX ## 13;		\
	typedef Pin<PORT_TYPE_NAME, 14> PIN_NAME_PREFIX ## 14;		\
	typedef Pin<PORT_TYPE_NAME, 15> PIN_NAME_PREFIX ## 15;		\
	typedef Pin<PORT_TYPE_NAME, 16> PIN_NAME_PREFIX ## 16;		\
	typedef Pin<PORT_TYPE_NAME, 17> PIN_NAME_PREFIX ## 17;		\
	typedef Pin<PORT_TYPE_NAME, 18> PIN_NAME_PREFIX ## 18;		\
	typedef Pin<PORT_TYPE_NAME, 19> PIN_NAME_PREFIX ## 19;		\
	typedef Pin<PORT_TYPE_NAME, 20> PIN_NAME_PREFIX ## 20;		\
	typedef Pin<PORT_TYPE_NAME, 21> PIN_NAME_PREFIX ## 21;		\
	typedef Pin<PORT_TYPE_NAME, 22> PIN_NAME_PREFIX ## 22;		\
	typedef Pin<PORT_TYPE_NAME, 23> PIN_NAME_PREFIX ## 23;		\
	typedef Pin<PORT_TYPE_NAME, 24> PIN_NAME_PREFIX ## 24;		\
	typedef Pin<PORT_TYPE_NAME, 25> PIN_NAME_PREFIX ## 25;		\
	typedef Pin<PORT_TYPE_NAME, 26> PIN_NAME_PREFIX ## 26;		\
	typedef Pin<PORT_TYPE_NAME, 27> PIN_NAME_PREFIX ## 27;		\
	typedef Pin<PORT_TYPE_NAME, 28> PIN_NAME_PREFIX ## 28;		\
	typedef Pin<PORT_TYPE_NAME, 29> PIN_NAME_PREFIX ## 29;		\
	typedef Pin<PORT_TYPE_NAME, 30> PIN_NAME_PREFIX ## 30;		\
	typedef Pin<PORT_TYPE_NAME, 31> PIN_NAME_PREFIX ## 31;

			DECLARE_PORT_PINS(PortA, Pa)
			DECLARE_PORT_PINS(PortB, Pb)
			DECLARE_PORT_PINS(PortC, Pc)
			DECLARE_PORT_PINS(PortD, Pd)
			DECLARE_PORT_PINS(PortE, Pe)
			DECLARE_PORT_PINS(PortF, Pf)
			DECLARE_PORT_PINS(PortG, Pg)
			DECLARE_PORT_PINS(PortH, Ph)
			DECLARE_PORT_PINS(PortI, Pi)
			DECLARE_PORT_PINS(PortJ, Pj)
			DECLARE_PORT_PINS(PortK, Pk)
		}
	}
}

#endif //EMBEDDED_IOPIN_H
