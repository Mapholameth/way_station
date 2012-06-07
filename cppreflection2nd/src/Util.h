#pragma once

#include <sstream>
#include <ostream>
using std::stringstream;
using std::ostringstream;

template <typename T>
class IsIntegral
{
public:
	enum { result = false };
};

#define D2D_DECLARE_INTEGRAL_TYPE(T)	\
	template <>	\
	class IsIntegral<T>	\
	{	\
	public:	\
		enum { result = true };	\
	};	\

D2D_DECLARE_INTEGRAL_TYPE(char)
D2D_DECLARE_INTEGRAL_TYPE(int)
D2D_DECLARE_INTEGRAL_TYPE(float)
D2D_DECLARE_INTEGRAL_TYPE(bool)

template<typename T>
class IsAbstract
{
        // todo: add assertions to ensure sizeof(char) == 1 and sizeof(short) == 2
        template<class U> static char test( U (*)[1] );
        template<class U> static short test( ... );

public:
        enum { result = sizeof( test<T>( 0 ) ) - 1 }; 
};


template<typename T, int A = IsAbstract<T>::result>
class Make
{
public:
    static T* New()
    {
            return new T();
    }
};


template<typename T>
class Make<T, 1>
{
public:
	static T* New()
	{
			throw("pizdec");                
	}
};

template<typename T>
struct identity
{
		typedef T type;
};


/**
* VariantConvert - contains implementations of functions which convert strings to arbitrary plain type, supported by stringstream.
*/


namespace VariantConvert
{
		template<typename T>
		T from_string_impl(const string &s, identity<T>)
		{
				T result;
				stringstream ss;
				ss >> std::noskipws;
				ss.str(s);
				ss >> result;
				return result;
		}


		bool from_string_impl(const string &s, identity<bool>)
		{
				// ios::boolalpha - have you looked for it, while trying to reinvent the wheel? // it doesn't suit our needs.. i don't remember why, though..
				if (s == "false" || s.empty())
						return false;


				long double result;
				stringstream ss;
				ss.str(s);
				ss >> result;


				if (ss.fail())
						return true;


				return ((int) result != 0);
		}


		string from_string_impl(const string &s, identity<string>)
		{
				return s;
		}

		template <typename T>
		string to_string_impl(const T& t)
		{
				ostringstream s;
				s << t;
				return s.str();
		}


}       //      namespace VariantConvert

template<typename T, int A = IsIntegral<T>::result>
class From
{
public:
	static T String(const string &s)
	{
			return VariantConvert::from_string_impl(s, identity<T>());
	}
};


template<typename T>
class From<T, 0>
{
public:
	static T String(const string &s)
	{
		throw "pizdec";
	}
};

template<typename T, int A = IsIntegral<T>::result>
class To
{
public:
	static string String(const T &value)
	{
		return VariantConvert::to_string_impl(value);
	}
};

template<typename T>
class To<T, 0>
{
public:
	static string String(const T &value)
	{
		throw "pizdec";
	}
};
