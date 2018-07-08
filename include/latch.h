#ifndef LATCH_H
#define LATCH_H

template<typename ClockPin, typename DataPin, typename LatchPin, unsigned ID, typename T = uint8_t>
class ThreePinLatch
{
	public:
		typedef T DataT;
		enum {Id = ID};
		enum {width = sizeof(DataT) * 8};

		static void write(T value)
		{
			_currentValue = value;
			for(uint8_t i = 0; i < width; ++i)
			{
				DataPin::set(value & 1);
				ClockPin::set();
				value >>= 1;
				ClockPin::clear();
			}
			LatchPin::set();
			LatchPin::clear();
		}

		static DataT read()
		{
			return _currentValue;
		}

		static void clearAndSet(DataT clearMask, DataT value)
		{
			write(_currentValue = (_currentValue & ~clearMask) | value);
		}

		static void set(DataT value)
		{
			write(_currentValue |= value);
		}

		static void clear(DataT value)
		{
			write(_currentValue &= ~value);
		}

		static void toggle(DataT value)
		{
			write(_currentValue ^= value);
		}

		static void writeDir(DataT value)
		{}

		static DataT readDir()
		{
			return 0xff;
		}

		static void setDir(DataT value)
		{}

		static void clearDir(DataT value)
		{}

		static void toggleDir(DataT value)
		{}

	protected:
		static DataT _currentValue;
};

template<typename ClockPin, typename DataPin, typename LatchPin, unsigned ID, typename T>
T ThreePinLatch<ClockPin, DataPin, LatchPin, ID, T>::_currentValue = 0;

#endif
