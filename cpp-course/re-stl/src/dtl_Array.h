#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <new>
#include <cassert>
#include "dtl_Utility.h"
#include "dtl_Allocator.h"
#include "dtl_Exception.h"

namespace dtl
{

template<typename DataType, typename AllocatorType = CDynAllocator<DataType>>
class vector
{
private:
	//typedef vector_base<DataType, AllocatorType> base;
	typedef vector<DataType, AllocatorType> self;
public:
	//typedef typename _Base::allocator_type allocator_type;
	typedef typename AllocatorType allocator_type;

	typedef DataType value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;

	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	//typedef random_access_iterator_tag _Iterator_category;

protected:
	AllocatorType Allocator;
	DataType *Data;

	void Resize(size_t NewElementsNumber)
	{
		Allocator.resize(NewElementsNumber);
		Data = Allocator.pointer();
	}

public:
// 	class iterator
// 	{
// 	private:
// 		vector *Array;
// 		unsigned int Index;
// 
// 		bool IsEnd() const
// 		{
// 			return Index >= Array->size();
// 		}
// 
// 		bool IsValid() const
// 		{
// 			return Array != NULL;
// 		}
// 
// 	public:
// 		iterator(vector *AArray = NULL) : Array(AArray), Index(-1){}
// 
// 		DataType& operator *()
// 		{
// 			if (!IsValid())
// 				throw CExceptionInvalidIteratorOperation();
// 			return Array->Data[Index];
// 		}
// 
// 		const DataType& operator *() const 
// 		{
// 			if (!IsValid())
// 				throw CExceptionInvalidIteratorOperation();
// 			return Array->Data[Index];
// 		}
// 
// 		iterator& operator++()
// 		{
// 			if (IsEnd())
// 				Index = 0;
// 			else
// 				(++Index);
// 			return *this;
// 		}
// 
// 		iterator& operator--()
// 		{
// 			if (IsEnd())
// 				Index = Array->size() - 1;
// 			else
// 				(--Index);
// 			return *this;
// 		}
// 
// 		iterator operator ++(int)
// 		{
// 			iterator TempIterator = *this;
// 			++*this;
// 			return TempIterator;
// 		}
// 
// 		iterator operator --(int)
// 		{
// 			iterator TempIterator = *this;
// 			--*this;
// 			return TempIterator;
// 		}
// 
// 		iterator operator +(int rhs) const
// 		{
// 			iterator TempIterator = *this;
// 			return TempIterator += rhs;
// 		}
// 
// 		iterator operator -(int rhs) const
// 		{
// 			iterator TempIterator = *this;
// 			return TempIterator -= rhs;
// 		}
// 
// 		iterator& operator +=(int rhs)
// 		{
// 			Index += rhs;
// 			return *this;
// 		}
// 
// 		iterator& operator -=(int rhs)
// 		{
// 			Index -= rhs;
// 			return *this;
// 		}
// 
// 		bool operator ==(const iterator &rhs) const
// 		{
// 			if (!IsValid() || !rhs.IsValid())
// 				throw CExceptionInvalidIteratorOperation();
// 			return Array == rhs.Array && (Index == rhs.Index || IsEnd() == rhs.IsEnd());
// 		}
// 
// 		bool operator !=(const iterator &rhs) const
// 		{
// 			return !(*this == rhs);
// 		}
// 
// 		int operator -(const iterator &rhs) const
// 		{
// 			if (!IsValid() || !rhs.IsValid() || Array != rhs.Array)
// 				throw CExceptionInvalidIteratorOperation();
// 			return Index - rhs.Index;
// 		}
// 	};
// 
// 	IMPLEMENT_CONTAINER_SEQ_TYPEDEFS
// 	typedef unsigned int KeyType;

	explicit vector(size_t ASize = 0) : Allocator(ASize)
	{		
		Data = Allocator.pointer();
	}

	vector(const vector &Array) : Allocator(0), Data(NULL)
	{
		Resize(Array.size());
		for(size_t i = 0; i < Array.size(); i++)
			Data[i] = Array[i];
	}

	~vector()
	{
		clear();
	}

	bool empty() const
	{
		return size() == 0;
	}

	void push_back(const DataType &NewElement)
	{
		Resize(size() + 1);
		Data[size() - 1] = NewElement;
	}

	void pop_back()
	{
		if (empty())
			throw CExceptionDeleteFromEmpty();
		Resize(size() - 1);
	}

	DataType& front()
	{
		if (empty())
			throw(CExceptionIndexOutOfBounds());
		return Data[0];
	}

	const DataType& front() const
	{
		if (empty())
			throw(CExceptionIndexOutOfBounds());
		return Data[0];
	}

	DataType& back()
	{
		if (empty())
			throw(CExceptionIndexOutOfBounds());
		return Data[size() - 1];
	}

	const DataType& back() const
	{
		if (empty())
			throw(CExceptionIndexOutOfBounds());
		return Data[size() - 1];
	}

	void clear()
	{
		Resize(0);
	}

	size_t size() const
	{
		return Allocator.size();
	}

	size_t capacity() const
	{
		return Allocator.capacity();
	}

	void reserve(unsigned int AElementsNumber)
	{
		Allocator.reserve(AElementsNumber);
	}

	void resize(size_t NewSize)
	{
		resize(NewSize);
	}

	vector& operator =(const vector &Operand)
	{
		if (this == &Operand)
			return *this;
		clear();
		new(this)vector(Operand);
		return *this;
	}

	DataType& operator [](int Index) // At first glance Index should be unsigned, but if so, I'll can't catch errors.
	{
		if (Index < 0)
			throw(CExceptionIndexOutOfBounds());
		// Decided to follow the semantics of "operator []" just like associative containers do
		if (Index >= size())
			Resize(Index + 1);
		return Data[Index];
	}

	const DataType& operator [](int Index) const
	{
		if (Index >= size() || Index < 0)
			throw(CExceptionIndexOutOfBounds());
		return Data[Index];
	}

	iterator begin()
	{
		return Data;//++iterator(this);
	}

	iterator end()
	{
		return Data + size();//iterator(this);
	}
	// won't work
	iterator insert(iterator position, const value_type &Element)
	{
		// Check iterator validity?
		if (position > begin() + size())
			throw CExceptionInvalidIteratorOperation();
		Resize(size() + 1);
		for (pointer i = position; i < position + size(); i++)
			*(i + 1) = *i;
		*position = Element;
		return position;
	}

	void insert(iterator position, size_type n, const value_type& x)
	{
		if (!position.IsValid())
			throw CExceptionInvalidIteratorOperation();
		for (unsigned int = position.Index; i < size(); i++)
			Data[i + n] = Data[i];
		for (unsigned int = position.Index; i < n; i++)
			Data[i] = x;
	}

	template <typename InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{
		for (InputIterator i = first; i != last; ++i)
		{
			
		}
	}
};

};

#endif	//	_CARRAY_H_