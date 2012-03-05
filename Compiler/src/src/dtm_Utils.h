#ifndef _DTM_UTILS_H_

#include <string>
#include <sstream>
using std::ostringstream;
using std::stringstream;
using std::string;
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;
#include <boost/numeric/conversion/cast.hpp>
using boost::numeric_cast;
using boost::numeric::bad_numeric_cast;
using boost::numeric::positive_overflow;
using boost::numeric::negative_overflow;

/**
*	fromhex - usage together with lexical cast:
*	unsigned int SomeInteger = lexical_cast <fromhex<unsigned int>>("0xdeadbeef");
*/

template <typename ElementT>
class fromhex
{
public:
	ElementT value;
	operator ElementT() const {return value;}
	friend std::istream& operator>>(std::istream& in, fromhex& out)
	{
		in >> std::hex >> out.value;
		return in;
	}
};

/**
*	EscapeSequenceToValue() expects valid escape sequence.	i.e. 
*	if we found \X0ff, then this function should get "X0FF"
*	and for \n, this function just gets "n"
*/

inline char EscapeSequenceToValue(const string &EscapeSequence)
{
	switch (EscapeSequence[0])
	{
	case 'a': return '\a';
	case 'b': return '\b';
	case 'f': return '\f';
	case 'n': return '\n';
	case 'r': return '\r';
	case 't': return '\t';
	case 'v': return '\v';
	case 'x': case 'X': return numeric_cast<unsigned char, unsigned int>(boost::lexical_cast
											< fromhex<unsigned int> >("0" + EscapeSequence));
	default: return EscapeSequence[0];
	}
}

inline unsigned long long int FromHexadecimalString(const string &Source)
{
	return boost::lexical_cast< fromhex<unsigned long long int> >(Source);
}

#define _DTM_UTILS_H_
#endif // _DTM_UTILS_H_