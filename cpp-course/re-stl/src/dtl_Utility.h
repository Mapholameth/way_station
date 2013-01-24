#ifndef _DTL_UTILITY_H_
#define _DTL_UTILITY_H_

#include <limits>
#include <cstdlib>

template <typename DataType>
struct Is
{
	static const bool concrete = std::numeric_limits<DataType>::is_specialized;
};

template<>
struct Is<void>
{
	static const bool concrete = true;	
};

#define IMPLEMENT_IS_CONCRETE(TYPE) \
	template<typename DataType> \
	struct Is<TYPE> \
	{ \
		static const bool concrete = true; \
	};

// using: Is<T>::concrete; 

//IMPLEMENT_IS_CONCRETE(const T)
IMPLEMENT_IS_CONCRETE(DataType*)
IMPLEMENT_IS_CONCRETE(const DataType*)
IMPLEMENT_IS_CONCRETE(DataType* const)
IMPLEMENT_IS_CONCRETE(const DataType* const)
IMPLEMENT_IS_CONCRETE(DataType&)
IMPLEMENT_IS_CONCRETE(const DataType&)

template<int MaxElements>
struct StaticLg 
{
	static const int value = 1 + StaticLg<MaxElements / 2>::value;
};

template<>
struct StaticLg<1>
{
	static const int value = 0;
};

template<int MaxElements, int B>
struct StaticLog {
	static const int value = 1 + StaticLog<MaxElements/B, B>::value;
};

template<int B>
struct StaticLog<1, B> {
	static const int value = 0;
};

inline float frand()
{
	return static_cast<float>(rand()) / RAND_MAX;
}

#define IMPLEMENT_CONTAINER_SEQ_TYPEDEFS \
	typedef DataType value_type;	\
	typedef size_t size_type;	\

#define IMPLEMENT_CONTAINER_ASSOC_TYPEDEFS \
	typedef DataType mapped_type;	\
	typedef KeyType key_type;	\
	typedef std::pair<const KeyType, DataType> value_type;	\

class CNullType{};
class CEmptyType
{
public:
	CEmptyType(){}
};

template <typename H, typename T>
class CTypeList
{
public:
	typedef H HeadType;
	typedef T TailType;
};

namespace DTL_TypeLists
{
	// Number of types in typelist
	template <typename T>
	class Length;

	template <>
	class Length<CNullType>
	{
	public:
		enum
		{
			Value = 0,
		};
	};

	template <typename H, typename T> 
	class Length<CTypeList<H, T>>
	{
	public:
		enum
		{
			Value = 1 + Length<T>::Value,
		};
	};

	// blah blah blah
	template <typename T, unsigned int Index>
	class TypeByIndex;
	
	template<typename H, typename T>
	class TypeByIndex<CTypeList<H, T>, 0>
	{
	public:
		typedef H Type;
	};

	template <typename H, typename T, unsigned int i>
	class TypeByIndex<CTypeList<H, T>, i>
	{
	public:
		typedef typename TypeByIndex<T, i - 1>::Type Type;
	};
}

#define DTL_TYPELIST_1(T1) CTypeList<T1, CNullType>
#define DTL_TYPELIST_2(T1, T2) CTypeList<T1, DTL_TYPELIST_1(T2) >
#define DTL_TYPELIST_3(T1, T2, T3) CTypeList<T1, DTL_TYPELIST_2(T2, T3) >
#define DTL_TYPELIST_4(T1, T2, T3, T4) CTypeList<T1, DTL_TYPELIST_3(T2, T3, T4) >

static DTL_TYPELIST_4(int, int, int , int) FourIntegers;
static const unsigned int  FourIntegersLength = DTL_TypeLists::Length<DTL_TYPELIST_4(int, int, int , int)>::Value;

#endif // _DTL_UTILITY_H_