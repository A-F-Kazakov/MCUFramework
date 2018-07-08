/**
 * author   k 
 * date     03.12.17.
 */

#ifndef VELO_NRF24L01_H
#define VELO_NRF24L01_H

class NRF24L01
{
	public:
		enum PowerLevel
		{
			RF24_PA_MIN = 0,
			RF24_PA_LOW,
			RF24_PA_HIGH,
			RF24_PA_MAX,
			RF24_PA_ERROR
		};
};

#endif //VELO_NRF24L01_H
