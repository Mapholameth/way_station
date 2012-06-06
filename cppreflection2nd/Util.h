#pragma once

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