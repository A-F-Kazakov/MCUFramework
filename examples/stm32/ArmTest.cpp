/**
 * @author	Казаков Андрей
 * @date		25.05.2017.
 */

#include "ioports.h"

using mcu::io::PortD;
using mcu::io::PortA;

using mcu::io::Configuration;
using mcu::io::DriverType;
using mcu::io::Speed;
using mcu::io::PullMode;

using greenLed = mcu::io::Pd12;
using redLed = mcu::io::Pd13;
using orangeLed = mcu::io::Pd14;
using blueLed = mcu::io::Pd15;

using hz = mcu::io::Pa0;

#define DELAY_VALUE 2000

int main()
{
	PortD::enable();
	PortA::enable();

	PortD::configure(0xF000, Configuration::Out);
	PortD::configure(0, DriverType::PushPull);
	PortD::configure(0, Speed::Fastest);

	hz::configure(Configuration::In);
	hz::configure(Speed::Fastest);
	hz::configure(DriverType::PushPull);
	hz::configure(PullMode::NoPullUp);

//	PortD::configure(0b00000000'00000000, PullMode::NoPullUp);

	greenLed::set();
	while(true)
	{
		if(hz::isSet())
			redLed::set();
		else
			redLed::clear();

		blueLed::set();
		for(int i = 0; i < DELAY_VALUE; ++i);
		blueLed::clear();
		for(int i = 0; i < DELAY_VALUE; ++i);
	}
}
