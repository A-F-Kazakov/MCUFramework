#ifndef PINLIST_H
#define PINLIST_H

#include <typelist.h>
#include <iopin.h>

namespace velo
{
	namespace mcu
	{
		using namespace velo::generic;
		using velo::generic::internal::Void;
		using velo::embedded::io::Pin;

		/**
		 * Обертка над Pin для сохранения порядкового номера в списке В\В
		 *
		 * @tparam TypeList	TPIN
		 * @tparam int 		POSITION
		 */

		template<class TPIN, uint8_t POSITION>
		struct PinWrapper
		{
			typedef TPIN Pin;
			enum{ position = POSITION };
		};

		/**
		 * Шаблон для создания списка линий В\В
		 *
		 * @tparam position
		 * @tparam H
		 * @tparam T
		 */

		template<int position, typename H = internal::Void, typename ...T>
		struct MakePinList
		{
			private:
				typedef typename MakePinList<position + 1, T...>::Result TailResult;
				enum{ PositionInList = position };
			public:
				typedef typename Append<PinWrapper<H, PositionInList>, TailResult>::type Result;
		};

		template<int Position>
		struct MakePinList<Position>{ typedef EmptyTypeList Result; };

		/**
		 * Составляет список портов из списка линий В\В
		 *
		 * @tparam TypeList TL
		 */

		template<typename TL>
		struct GetPorts;

		template<>
		struct GetPorts<EmptyTypeList>{ typedef EmptyTypeList Result; };

		template<typename H, typename ...T>
		struct GetPorts<TypeList<H, T...>>
		{
			private:
				typedef typename H::Pin::Port                     Port;
				typedef typename GetPorts<TypeList<T...>>::Result L1;
			public:
				typedef typename AppendUnique<Port, L1>::type Result;
		};

		/**
		 * Составляет список линий В\В из смешанного списка по указанному порту
		 *
		 * @tparam TypeList TList	- Список линий
		 * @tparam T 					- Тип порта для поиска
		 */

		template<typename TList, typename T>
		struct GetPinsWithPort;

		template<typename T>
		struct GetPinsWithPort<EmptyTypeList, T>{ typedef EmptyTypeList Result; };

		template<typename T, uint8_t N, uint8_t M, typename ...Tail>
		struct GetPinsWithPort<TypeList<PinWrapper<Pin<T, N>, M>, Tail...>, T>
		{
			private:
				typedef typename GetPinsWithPort<TypeList<Tail...>, T>::Result L1;
			public:
				typedef typename Append<TypeList<PinWrapper<Pin<T, N>, M>>, L1>::type Result;
		};

		template<typename T, typename Head, typename ...Tail>
		struct GetPinsWithPort<TypeList<Head, Tail...>, T>
		{
			typedef typename GetPinsWithPort<TypeList<Tail...>, T>::Result Result;
		};

		/**
		 * Вычисляем битовую маску
		 *
		 * @tparam TypeList TList - линии В\В одного порта
		 */

		template<typename TList>
		struct GetPortMask;

		template<>
		struct GetPortMask<EmptyTypeList>{ enum{ value = 0 }; };

		template<typename Head, class ...Tail>
		struct GetPortMask<TypeList<Head, Tail...>>
		{
			enum{ value = (1 << Head::Pin::number) | GetPortMask<TypeList<Tail...>>::value };
		};

		/**
		 * ?
		 * @tparam TList
		 */

		template<class TList>
		struct GetValueMask;

		template<>
		struct GetValueMask<EmptyTypeList> { enum{ value = 0 }; };

		template<class Head, class ...Tail>
		struct GetValueMask<TypeList<Head, Tail...>>
		{
			enum{ value = (1 << Head::position) | GetValueMask<TypeList<Tail...>>::value };
		};

		/**
		 * Узнаем, идут ли линии В\В по порядку
		 *
		 * @tparam TList
		 */

		template<class TList>
		struct IsSerial;

		template<>
		struct IsSerial<EmptyTypeList>
		{
			// пустой список последователен
			enum{ value = 1 };
			// номер текущей линии
			enum{ PinNumber = -1 };
			// признак конца списка
			enum{ EndOfList = 1 };
		};

		template<class Head, class ...Tail>
		struct IsSerial<TypeList<Head, Tail...>>
		{
			typedef IsSerial<TypeList<Tail...>> I;

			enum{ PinNumber = Head::Pin::number };
			enum{ EndOfList = 0 };
			enum{ value = ((PinNumber == I::PinNumber - 1) && I::value) || I::EndOfList };
		};

		/**
		 *
		 * @tparam TypeList TList - список линий одного порта
		 */

		template<class TList>
		struct PinWriteIterator;

		template<>
		struct PinWriteIterator<EmptyTypeList>
		{
			template<class DataType> static uint8_t appendValue(const DataType &value)
			{ return 0; }

			template<class DataType> static inline DataType appendReadValue(uint8_t portValue, DataType)
			{ return 0; }
		};

		template<class Head, class ...Tail>
		struct PinWriteIterator<TypeList<Head, Tail...>>
		{
			template<class DataType> static inline uint8_t appendValue(const DataType &value)
			{
				// проверяем, если линии в порту расположены последовательно
				// если часть линий в середине списка будет расположена последовательно, то
				// это условие не выполнется, так, что есть ещё простор для оптимизации.
				if(IsSerial<TypeList<Head, Tail...> >::value)
				{
					// сдвигаем значение на нужное число бит и накладываем не него маску
					if((int) Head::position > (int) Head::Pin::number)
						return (value >> ((int) Head::position - (int) Head::Pin::number)) &
								 GetPortMask<TypeList<Head, Tail...>>::value;
					else
						return (value << ((int) Head::Pin::number - (int) Head::position)) &
								 GetPortMask<TypeList<Head, Tail...>>::value;
				}

				uint8_t result = 0;

				if((int) Head::position == (int) Head::Pin::number)
					result |= value & (1 << Head::position);
				else
					// это условие будет вычисляться во время выполнения программы
					if(value & (1 << Head::position))
						result |= (1 << Head::Pin::number);
				// рекурсивно обрабатываем оставшиеси линии в списке
				return result | PinWriteIterator<TypeList<Tail...>>::appendValue(value);
			}

			template<class DataType> static inline DataType appendReadValue(uint8_t portValue, DataType dummy)
			{
				if(IsSerial<TypeList<Head, Tail...> >::value)
				{
					if((int) Head::position > (int) Head::Pin::Number)
						return (portValue >> ((int) Head::position - (int) Head::Pin::Number)) &
								 GetValueMask<TypeList<Head, Tail...> >::value;
					else
						return (portValue << ((int) Head::Pin::Number - (int) Head::position)) &
								 GetValueMask<TypeList<Head, Tail...> >::value;
				}

				DataType value = 0;
				if((int) Head::position == (int) Head::Pin::Number)
					value |= portValue & (1 << Head::position);
				else
					if(portValue & (1 << Head::Pin::Number))
						value |= (1 << Head::position);

				return value | PinWriteIterator<TypeList<Tail...>>::appendReadValue(portValue, dummy);
			}
		};

		/**
		 *
		 * @tparam PortList
		 * @tparam PinList
		 */

		template<typename PortList, typename PinList>
		struct PortWriteIterator;

		template<class PinList>
		struct PortWriteIterator<EmptyTypeList, PinList>
		{
			template<class DataType> static void write(DataType value)
			{}

			template<class DataType> static void set(DataType value)
			{}

			template<class DataType> static void clear(DataType value)
			{}

			template<class DataType> static DataType readPin(DataType)
			{ return 0; }

			template<class DataType> static void writeDir(DataType value)
			{}

			template<class DataType> static void setDir(DataType value)
			{}

			template<class DataType> static void clearDir(DataType value)
			{}

			template<class DataType> static DataType readOut(DataType dummy)
			{ return 0; }
		};

		template<typename Head, typename ...Tail, typename PinList>
		struct PortWriteIterator<TypeList<Head, Tail...>, PinList>
		{
			typedef typename GetPinsWithPort<PinList, Head>::Result Pins;
			enum{ Mask = GetPortMask<Pins>::value };
			typedef Head                                            Port;

			template<typename DataType> static void write(DataType value)
			{
				// проецируем биты из входного значения в соответствующие биты порта
				// как это реализованно увидим дальше
				const DataType result = PinWriteIterator<Pins>::appendValue(value);

				// если кол-во бит в записываемом значении совпадает с шириной порта,
				// то записываем порт целиком.
				// это условие вычислится во время компиляции
				if((int) Length<Pins>::value == (int) Port::width)
					Port::write(result);
				else
					// PORT = PORT & Mask | result;
					Port::clearAndSet(Mask, result);

				// рекурсивно обрабатываем остальные порты в списке
				PortWriteIterator<TypeList<Tail...>, PinList>::write(value);
			}

			template<class DataType> static void set(DataType value)
			{
				uint8_t result = PinWriteIterator<Pins>::appendValue(value);
				Port::Set(result);

				PortWriteIterator<TypeList<Tail...>, PinList>::set(value);
			}

			template<class DataType> static void clear(DataType value)
			{
				uint8_t result = PinWriteIterator<Pins>::appendValue(value);
				Port::Clear(result);

				PortWriteIterator<TypeList<Tail...>, PinList>::clear(value);
			}

			template<class DataType> static void writeDir(DataType value)
			{
				uint8_t result = PinWriteIterator<Pins>::appendValue(value);
				if((int) Length<Pins>::value == (int) Port::Width)
					Port::DirWrite(result);
				else
				{
					Port::DirClear(Mask);
					Port::DirSet(result);
				}

				PortWriteIterator<TypeList<Tail...>, PinList>::writeDir(value);
			}

			template<class DataType> static void setDir(DataType value)
			{
				uint8_t result = PinWriteIterator<Pins>::appendValue(value);
				Port::DirSet(result);

				PortWriteIterator<TypeList<Tail...>, PinList>::setDir(value);
			}

			template<class DataType> static void clearDir(DataType value)
			{
				uint8_t result = PinWriteIterator<Pins>::appendValue(value);
				Port::DirClear(result);

				PortWriteIterator<TypeList<Tail...>, PinList>::clearDir(value);
			}

			template<class DataType> static DataType readPin(DataType dummy)
			{
				uint8_t  portValue = Port::PinRead();
				DataType value     = PinWriteIterator<Pins>::appendReadValue(portValue, dummy);
				return value | PortWriteIterator<TypeList<Tail...>, PinList>::readPin(dummy);
			}

			template<class DataType> static DataType readOut(DataType dummy)
			{
				uint8_t  portValue = Port::Read();
				DataType value     = PinWriteIterator<Pins>::appendReadValue(portValue, dummy);
				return value | PortWriteIterator<TypeList<Tail...>, PinList>::readOut(dummy);
			}
		};

		namespace internal
		{
			template<bool Short>
			struct SelectSize;

			template<>
			struct SelectSize<false> { typedef uint32_t Result; };

			template<>
			struct SelectSize<true> { typedef uint8_t Result; };
		}

		template<typename PINS>
		struct PinSet
		{
			public:
				typedef PINS PinTypeList;

				typedef typename GetPorts<PINS>::Result Ports;

				enum{ length = Length<PINS>::value };

				typedef typename internal::SelectSize<length <= 8>::Result DataType;

				//записать значение в список линий
				static void write(DataType value)
				{
					PortWriteIterator<Ports, PINS>::write(value);
				}

				template<uint8_t Num>
				class Take: public PinSet<typename TakeFirstN<PINS, Num>::type> {};

				template<uint8_t Num>
				class Skip: public PinSet< typename SkipFirst<PINS, Num>::type> {};

				template<uint8_t StartIndex, uint8_t Size>
				class Slice: public PinSet
				<typename SkipFirst< typename TakeFirstN<PINS, StartIndex + Size>::type, StartIndex>::type>
				{ static_assert(StartIndex + Size <= length, "Index out of bounds"); };

				// Лайвхак до выявления косяков
				template<uint8_t PIN>
				class Pin : public TypeAt<PINS, (length - 1) - PIN>::type::Pin{};

				static DataType read()
				{
					typedef PortWriteIterator<Ports, PINS> iter;
					return iter::readOut(DataType(0));
				}

				static void set(DataType value)
				{
					PortWriteIterator<Ports, PINS>::set(value);
				}

				static void clear(DataType value)
				{
					PortWriteIterator<Ports, PINS>::clear(value);
				}

				static DataType readPin()
				{
					typedef PortWriteIterator<Ports, PINS> iter;
					return iter::readPin(DataType(0));
				}

				static void writeDir(DataType value)
				{
					PortWriteIterator<Ports, PINS>::writeDir(value);
				}

				static void setDir(DataType value)
				{
					PortWriteIterator<Ports, PINS>::setDir(value);
				}

				static void clearDir(DataType value)
				{
					PortWriteIterator<Ports, PINS>::clearDir(value);
				}
		};

		template<typename H, typename ...T>
		struct PinList : public PinSet<typename MakePinList<0, H, T...>::Result>{};
	}
}

#endif // PINLIST_H