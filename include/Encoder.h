/**
 * author   k 
 * date     17.11.17.
 */

#ifndef VELO_ENCODER_H
#define VELO_ENCODER_H

#warning "under development"

namespace velo
{
	namespace mcu
	{
		template<typename DataT, typename Pins1, typename Pins2>
		class Encoder
		{
			static_assert(Pins1::Length == Pins2::Length);

			public:
				typedef typename Pins1::DataType PortType;
				static const unsigned Channels = Pins1::Length;

				static void initiate()
				{
					Pins1::template setConfiguration<Pins1::PullUpOrDownIn>();
					Pins2::template setConfiguration<Pins1::PullUpOrDownIn>();
					Pins1::template write<typename Pins1::DataT(-1)>();
					Pins2::template write<typename Pins2::DataT(-1)>();
				}

				static inline void captureHandler()
				{
					PortType y1 = Pins1::PinRead();
					PortType y2 = Pins2::PinRead();

					PortType fwd  = Detect(_x1, _x2, y1, y2);
					PortType back = Detect(_x2, _x1, y2, y1);

					_x1 = y1;
					_x2 = y2;

					volatile DataT * ptr = _value;
					for(uint8_t i = Channels; i; --i)
					{
						//if(fwd & 1)
						//	 (*ptr) ++;
						//else
						//if(back & 1)
						//	(*ptr) --;
						*ptr = *ptr + (fwd & 1) - (back & 1);
						ptr++;
						fwd >>= 1;
						back >>= 1;
					}
				}

				static DataT value(unsigned  index)
				{
					return _value[index];
				}

			private:
				static volatile DataT _value[Pins1::Length];
				static PortType _x1, _x2;

				static inline PortType Detect(PortType x1, PortType x2, PortType y1, PortType y2)
				{
					//return ~(x1 | y2) & (x2 ^ y1) | x1 & y2 & ~(x2 & y1);
					return (x2 ^ y1) &~ (x1 ^ y2);
				}
		};


		template<class DataT, class Pins1, class Pins2>
		volatile DataT Encoder<DataT, Pins1, Pins2>::_value[Pins1::Length];

		template<class DataT, class Pins1, class Pins2>
		typename Encoder<DataT, Pins1, Pins2>::PortType Encoder<DataT, Pins1, Pins2>::_x1;

		template<class DataT, class Pins1, class Pins2>
		typename Encoder<DataT, Pins1, Pins2>::PortType Encoder<DataT, Pins1, Pins2>::_x2;
	}
}

#endif //VELO_ENCODER_H
