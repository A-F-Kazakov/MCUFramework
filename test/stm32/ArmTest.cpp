/**
 * @author	K
 * @date		25-05-2017.
 */

#define USE_SPI1

#include "ioports.h"
#include "Button.h"
#include "Spi.h"

#define DELAY_VALUE 10000000

using namespace velo::mcu;
using namespace velo::mcu::io;

typedef Pd12 greenLed;
typedef Pd13 redLed;
typedef Pd14 orangeLed;
typedef Pd15 blueLed;

typedef Button<Pa0> Btn;

//typedef PinList<Pd12, Pd13, Pd14, Pd15> leds;

int main()
{
	Spi1::initiate(internal::SpiBase::ClockDivider::Div2);

	Spi1::read();

	PortD::enable();
	PortA::enable();

	PortD::configure(0xF000, Configuration::Out);
	PortD::configure(0, DriverType::PushPull);
	PortD::configure(0, Speed::Fastest);

	Pa0::configure(Configuration::In);
	Pa0::configure(Speed::Fastest);
	Pa0::configure(DriverType::PushPull);
	Pa0::configure(PullMode::NoPullUp);

//	PortD::configure(0b00000000'00000000, PullMode::NoPullUp);

	greenLed::set();
	while(true)
	{
		if(Pa0::isSet())
			redLed::set();
		else
			redLed::clear();

		blueLed::set();
		for(int i = 0; i < DELAY_VALUE; ++i);
		blueLed::clear();
		for(int i = 0; i < DELAY_VALUE; ++i);
	}
}
