/**
 * author   k 
 * date     17.11.17.
 */

#ifndef VELO_POWER_H
#define VELO_POWER_H

#warning "under development"

namespace velo
{
	namespace mcu
	{
		class Power
		{
			public:
				// Stop the CPU but all peripherals remains active.
				static inline void off();

				// Stop the CPU but all peripherals running at lower freq if supported by traget MCU, if not its equ to CpuSleep.
				static inline void sleepLowFreq();

				// Stop the CPU and all peripherals except that running asynchronously.
				//	Wake up only by async events such as external interrupt,
				//	or interrupts from asynchronously running modules such as timers with external clock.
				//	External Reset or Watchdog Reset.
				static inline void asyncPeripheryOnly();

				// Stop the CPU and all peripherals clocks. Main clock remains running for faster wake up.
				//	If not supported by target MCU this mode is equ to PowerDown
				//	Wake up only by async events such as external interrupt.
				//	External Reset or Watchdog Reset.
				static inline void standby();

				// Stop the CPU and all peripherals clocks. Main clock is source disabled.
				//	Wake up only by async events such as external interrupt.
				//	External Reset or Watchdog Reset.
				static inline void powerDown();

				// Need to be called from interrupts to exit sleep mode on some platforms
				static inline void exitSleepModeIrq();

				// Get supply voltage in 1/10 Volt units if supported by hardware.
				static inline unsigned getVdd();

				static inline bool detect();
		};
	}
}

#endif //VELO_POWER_H
