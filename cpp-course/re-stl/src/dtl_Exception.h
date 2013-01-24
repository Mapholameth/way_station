#ifndef _DTL_EXCEPTION_H_
#define _DTL_EXCEPTION_H_

#include <stdexcept>

struct CExceptionDeleteFromEmpty : public std::runtime_error 
{
	CExceptionDeleteFromEmpty() : std::runtime_error("Trying to delete element from empty Container.") {}
};

struct CExceptionIndexOutOfBounds : public std::runtime_error 
{
	CExceptionIndexOutOfBounds() : std::runtime_error("Index out of bounds") {}
};

struct CExceptionDeleteStartNode : public std::runtime_error
{
	CExceptionDeleteStartNode() : std::runtime_error("Trying to delete start node from the list.") {}
};

struct CExceptionInvalidOperation : public std::runtime_error
{
	CExceptionInvalidOperation() : std::runtime_error("Some invalid operation happened somewhere. Good luck finding it.") {}
};

struct CExceptionInvalidIteratorOperation : public std::runtime_error
{
	CExceptionInvalidIteratorOperation() : std::runtime_error("Somewhere an invalid operation was performed on some iterator. Good luck finding it.") {}
};



#endif	//	_DTL_EXCEPTION_H_