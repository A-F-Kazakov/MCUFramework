/**
 * @author	K
 * @date		10-06-2017.
 */

#ifndef EMBEDDED_BUTTON_H
#define EMBEDDED_BUTTON_H

namespace velo
{
	namespace mcu
	{
		template<typename Pin>
		class Button
		{
			public:
				static void initiate()
				{
					Pin::setDirRead();
				}
//
//				static void interrupt()
//				{
//					if(Pin::isSet())
//					{
//						if(lockCounter < 10)
//						{
//							++lockCounter;
//							return;
//						}
//
//						locked = true;
//					}
//					else
//					{
//						if(lockCounter)
//						{
//							--lockCounter;
//							return;
//						}
//
//						locked = false;
//						clicked = true;
//					}
//				}
//
//				static isClicked()
//				{
//					return clicked;
//				}

			private:
//				static uint8_t lockCounter = 0;
//				static bool locked = false;
//				static bool clicked = false;
		};

	}
}

#endif //EMBEDDED_BUTTON_H
