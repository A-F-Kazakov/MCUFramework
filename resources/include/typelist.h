/**
 * @author	Казаков Андрей
 * @date		09.05.2017
 */

#ifndef TESTS_TYPELIST_H
#define TESTS_TYPELIST_H

#include "arch.h"

#ifdef OS_LINUX

#include <cxxabi.h>

#elif defined(OS_WIN)

#include <type_traits>
#include <ostream>
#include <typeinfo>

#else

namespace std
{
	/// Define a member typedef @c type to one of two argument types.
	template<bool _Cond, typename _Iftrue, typename _Iffalse>
	struct conditional{ typedef _Iftrue type; };

	/// integral_constant
	template<typename _Tp, _Tp __v>
	struct integral_constant
	{
		static constexpr _Tp value = __v;
		typedef _Tp value_type;
		typedef integral_constant<_Tp, __v> type;

		constexpr operator value_type() const{ return value; }

#if __cplusplus > 201103L

#define __cpp_lib_integral_constant_callable 201304

		constexpr value_type operator()() const{ return value; }

#endif
	};

	/// The type used as a compile-time boolean with true value.
	typedef integral_constant<bool, true> true_type;

	/// The type used as a compile-time boolean with false value.
	typedef integral_constant<bool, false> false_type;

	/// is_same
	template<typename, typename>
	struct is_same : public false_type{};

	template<typename _Tp>
	struct is_same<_Tp, _Tp> : public true_type{};
}

#endif

namespace velo
{
	namespace generic
	{
		namespace internal
		{
			struct Void{};
		}

		template<typename ...Args>
		struct TypeList
		{
			typedef internal::Void Head;
			typedef internal::Void Tail;
		};

		typedef TypeList<> EmptyTypeList;

		template<typename H, typename ...T>
		struct TypeList<H, T...>
		{
			typedef H              Head;
			typedef TypeList<T...> Tail;
		};

// isEmpty

		template<typename TL>
		struct IsEmpty : std::true_type{};

		template<>
		struct IsEmpty<TypeList<internal::Void, internal::Void>> : std::true_type{};

		template<typename ...Args>
		struct IsEmpty<TypeList<Args...>> :
				std::integral_constant<bool,
											  std::is_same<typename TypeList<Args...>::Head,
																internal::Void>::value &&
											  IsEmpty<typename TypeList<Args...>::Tail>::value>{};

// Contains TODO: broken

//		template<typename T, typename TL> struct Contains : std::false_type{};
//
//		template<typename ...Args> struct Contains<internal::Void, Args...> : std::false_type{};
//
//		template<typename T, typename ...Args>
//		struct Contains<T, TypeList<Args...>> :
//				std::integral_constant<bool,
//											  std::is_same<typename TypeList<Args...>::Head, T>::value ||
//											  Contains<T, typename TypeList<Args...>::Tail>::value>
//		{
//		};

// Lenght

		template<typename TL>
		struct Length : std::integral_constant<unsigned int, 0>{};

		template<typename ...Args>
		struct Length<TypeList<Args...>> :
				std::integral_constant<unsigned int,
											  IsEmpty<TypeList<Args...>>::value ? 0 : 1 +
																									Length<typename TypeList<
																											Args...>::Tail>::value>
		{
		};

//TypeAt

		template<class TList, unsigned int index>
		struct TypeAt;

		template<class Head, class ...Tail>
		struct TypeAt<TypeList<Head, Tail...>, 0>
		{
			typedef Head type;
		};

		template<class Head, class ...Tail, unsigned int index>
		struct TypeAt<TypeList<Head, Tail...>, index>
		{
			typedef typename TypeAt<TypeList<Tail...>, index - 1>::type type;
		};

// print


		std::ostream& operator<<(std::ostream& ostr, EmptyTypeList)
		{
			ostr << "{}";
			return ostr;
		}

		template<typename TL>
		void PrintTypeListHelper(TL, std::ostream& ostr){}

		template<typename T>
		void PrintTypeListHead(T, std::ostream& ostr)
		{
			int status = 0;
			ostr << abi::__cxa_demangle(typeid(T).name(), 0, 0, &status) << ",";
		}

		template<typename ...Args>
		void PrintTypeListHead(TypeList<Args...> tl, std::ostream& ostr){ ostr << tl; }

		template<typename Head, typename ...Args>
		void PrintTypeListHelper(TypeList<Head, Args...>, std::ostream& ostr)
		{
			PrintTypeListHead(Head(), ostr);
			if(!IsEmpty<TypeList<Args...>>::value)
			{
				ostr << ' ';
				PrintTypeListHelper<Args...>(TypeList<Args...>(), ostr);
			}
		}

		template<typename ...Args>
		std::ostream& operator<<(std::ostream& ostr, TypeList<Args...> tl)
		{
			ostr << '{';
			PrintTypeListHelper(tl, ostr);
			ostr << "\b}";
			return ostr;
		}

// Append

		template<typename TOrTL2, typename TL>
		struct Append{};

		template<typename T, typename ...Args>
		struct Append<T, TypeList<Args...>>
		{
			typedef TypeList<Args..., T> type;
		};

		template<typename ...Args1, typename ...Args2>
		struct Append<TypeList<Args1...>, TypeList<Args2...>>
		{
			typedef TypeList<Args2..., Args1...> type;
		};

		template<typename T, typename TL>
		struct Add{};

		template<typename T, typename ...Args>
		struct Add<T, TypeList<Args...>>
		{
			typedef TypeList<Args..., T> type;
		};

// isIn

		template<typename TElement, typename TList>
		struct IsIn
		{
			typedef std::false_type type;
			enum{ value = type::value };
		};

		template<typename TElement, typename... TTail>
		struct IsIn<TElement, TypeList<TElement, TTail...>>
		{
			typedef std::true_type type;
			enum{ value = type::value };
		};

		template<typename TElement, typename THead, typename... TTail>
		struct IsIn<TElement, TypeList<THead, TTail...>>
		{
			typedef typename IsIn<TElement, TypeList<TTail...>>::type type;
			enum{ value = type::value };
		};

// Append unique

		template<typename TNew,
					typename TList,
					typename Tis_duplicate = typename IsIn<TNew, TList>::type>
		struct AppendUnique;

		template<typename TNew, typename... TList>
		struct AppendUnique<TNew, TypeList<TList...>, std::true_type>
		{
			typedef TypeList<TList...> type;
		};

		template<typename TNew, typename... TList>
		struct AppendUnique<TNew, TypeList<TList...>, std::false_type>
		{
			typedef TypeList<TNew, TList...> type;
		};

// remove all

		template<typename TOrTL2, typename TL>
		struct RemoveAll{};

		template<typename T, typename ...Args>
		struct RemoveAll<T, TypeList<Args...>>
		{
			private:
				typedef typename RemoveAll<T, typename TypeList<Args...>::Tail>::type Removed;
				typedef typename TypeList<Args...>::Head                              Head;

			public:
				typedef typename std::conditional<std::is_same<Head, T>::value,
															 Removed,
															 typename Append<Removed,
																				  TypeList<Head>>::type>::type type;
		};

		template<typename T, typename Head>
		struct RemoveAll<T, TypeList<Head>>
		{
			typedef typename std::conditional<std::is_same<Head, T>::value,
														 EmptyTypeList,
														 TypeList<Head>>::type type;
		};

		template<typename T>
		struct RemoveAll<T, EmptyTypeList>
		{
			typedef EmptyTypeList type;
		};

// RemoveDuplicates

		template<typename TL>
		struct RemoveDuplicates{};

		template<>
		struct RemoveDuplicates<EmptyTypeList>
		{
			typedef EmptyTypeList type;
		};

		template<typename ...Args>
		struct RemoveDuplicates<TypeList<Args...>>
		{
			private:
				typedef TypeList<Args...>                                    TL;
				typedef typename RemoveAll<typename TL::Head,
													typename TL::Tail>::type          HeadRemovedFromTail;
				typedef typename RemoveDuplicates<HeadRemovedFromTail>::type TailWithoutDuplicates;
			public:
				typedef typename Append<TailWithoutDuplicates, TypeList<typename TL::Head>>::type type;
		};

//		TakeLast

		template<typename T>
		struct TakeLast;

		template<typename H, typename ...T>
		struct TakeLast<TypeList<H, T...>>
		{
			typedef typename TakeLast<TypeList<T...>>::type type;
		};

		template<typename T>
		struct TakeLast<TypeList<T>>{ typedef T type; };

//		TakeFirst

		template<typename T>
		struct TakeFirst;

		template<typename H, typename ...T>
		struct TakeFirst<TypeList<H, T...>>{ typedef H type; };

//		TakeFirstN

		template<typename TList, uint8_t Num>
		struct TakeFirstN;

		template<>
		struct TakeFirstN<EmptyTypeList, 0>{ typedef EmptyTypeList type; };

		template<typename H, typename ...T>
		struct TakeFirstN<TypeList<H, T...>, 0>{ typedef EmptyTypeList type; };

		template<typename H, typename ...T, uint8_t Num>
		struct TakeFirstN<TypeList<H, T...>, Num>
		{
			typedef typename TakeFirstN<TypeList<T...>, Num - 1>::type l1;
			typedef typename Append<l1, TypeList<H>>::type             type;
		};

//		SkipFirst

		template<typename TList, uint8_t Num>
		struct SkipFirst;

		template<>
		struct SkipFirst<EmptyTypeList, 0>{ typedef EmptyTypeList type; };

		template<typename H, typename ...T>
		struct SkipFirst<TypeList<H, T...>, 0>{ typedef TypeList<H, T...> type; };

		template<typename H, typename ...T, uint8_t Num>
		struct SkipFirst<TypeList<H, T...>, Num>
		{
			typedef typename SkipFirst<TypeList<T...>, Num - 1>::type type;
		};
	}
}
#endif //TESTS_TYPELIST_H
