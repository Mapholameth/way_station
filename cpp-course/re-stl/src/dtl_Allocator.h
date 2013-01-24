#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include <cstdlib>
#include <cassert>
#include <malloc.h>

/**
*	CSomeAllocator:
*	GetPointer(): returns pointer to allocated memory.
*	Resize(n): Stands for Allocate + Reallocate + Deallocate at the same time.
*		i.e. first time invokation Allocates memory, next calls used to reallocate memory and
*		if we want to ResizeTo(0) then it Deallocates memory.
*	Reserve() ensures given capacity
*/

template<typename DataType, unsigned int GrowthFactor = 2, unsigned int ReduceCriteria = 4>
class CDynAllocator
{
private:
	size_t Capacity;
	DataType* Pointer;
	size_t Size;

	void Deallocate()
	{
		free(Pointer), Pointer = NULL, Capacity = 0, Size = 0;
	}

	void InvokeDestructors_(size_t from, size_t to)
	{
		if (Is<DataType>::concrete)
			return;
		for(size_t i = from; i < to; i++)
			(Pointer + i)->DataType::~DataType();
	}

	void InvokeConstructors_(size_t from, size_t to)
	{
		if (Is<DataType>::concrete)
			return;
//  		try
//  		{
			for(size_t i = from; i < to; i++)
				new(Pointer + i)DataType();
// 		}
// 		catch(...)
// 		{
// 			for(size_t i = 0; i < ElementsNumber; i++)
// 				(Data + i)->DataType::~DataType();	// ORLY we have to call dtor if ctor raised exception?
// 			Deallocate();
// 			throw;
// 		}
	}

public:
	typedef DataType value_type;
	typedef value_type* PointerType;
	typedef const value_type* ConstPointerType;
	typedef value_type& ReferenceType;
	typedef const value_type& ConstReferenceType;
	typedef std::size_t SizeType;
	//typedef std::ptrdiff_t DifferenceType;
	typedef ptrdiff_t DifferenceType;
	

	explicit CDynAllocator(size_t ASize = 0) : Capacity(0), Pointer(NULL), Size(0)
	{
		resize(ASize);
	}

	~CDynAllocator()
	{
		Deallocate();
	}

	size_t capacity() const
	{
		return Capacity;
	}

	size_t size() const
	{
		return Size;
	}

	DataType* pointer()
	{
		return Pointer;
	}

	void reserve(size_t ACapacity)
	{
		if (ACapacity <= Capacity)
			return;
		Capacity = ACapacity;
		DataType* NewPointer = static_cast<DataType*>(calloc(Capacity, sizeof(DataType)));
#undef min
		std::swap(Pointer, NewPointer);
		InvokeConstructors_(0, Size);
		std::swap(Pointer, NewPointer);
		for(size_t i = 0; i < Size; i++)
			NewPointer[i] = Pointer[i];
		InvokeDestructors_(0, Size);
		free(Pointer);
		Pointer = NewPointer;
	}

	void resize(size_t ASize)
	{
		if (ASize == 0)
		{
			InvokeDestructors_(0, Size);
			Deallocate();
			return;
		}

		size_t OldCapacity = Capacity;

		if (ASize > Capacity)
			Capacity = ASize * GrowthFactor;
		else if (ASize * ReduceCriteria <= Capacity)
			Capacity /= ReduceCriteria;

		if (Capacity == OldCapacity)
		{
			InvokeConstructors_(Size, ASize);
			InvokeDestructors_(ASize, Size);
			Size = ASize;
			return;
		}

 		DataType* NewPointer = static_cast<DataType*>(calloc(Capacity, sizeof(DataType)));
#undef min
		size_t limit = std::min(Size, ASize);
		std::swap(Pointer, NewPointer);
		InvokeConstructors_(0, limit);
		std::swap(Pointer, NewPointer);
 		for(size_t i = 0; i < limit; i++)
 			NewPointer[i] = Pointer[i];
		InvokeDestructors_(0, Size);
		free(Pointer);
		Pointer = NewPointer;
		InvokeConstructors_(Size, ASize);
		Size = ASize;
		return;
	}
};

/**
*	CFixedAllocator: Tries to allocate memory in stack. Can't dynamically allocate more than it already has.	
*/

template<typename DataType, size_t MaxElements>
class CFixedAllocator
{
private:
	unsigned char Data[sizeof(DataType) * MaxElements];
	size_t Size;
public:
	CFixedAllocator()
	{
		memset(Data, 0, sizeof(DataType) * MaxElements);
	}
	explicit CFixedAllocator(size_t Size)
	{
		if (Size > MaxElements)
			throw std::bad_alloc();
		memset(Data, 0, sizeof(DataType) * /*Size*/ MaxElements);
	}
	CFixedAllocator& operator =(const CFixedAllocator &rhs)
	{
		memcpy(Data, rhs.Data, sizeof(DataType) * MaxElements);
		return *this;
	}
	size_t GetCapacity() const
	{
		return MaxElements;
	}
	DataType* GetPointer()
	{
		return reinterpret_cast<DataType*>(Data);
	}
	void Resize(size_t Size)
	{
		if (Size > MaxElements)
			throw std::bad_alloc();
	}
};

/* Sample interface:
template<typename DataType>
class CDynAllocator
{
public:
	typedef DataType value_type;
	typedef value_type* PointerType;
	typedef const value_type* ConstPointerType;
	typedef value_type& ReferenceType;
	typedef const value_type& ConstReferenceType;
	typedef std::size_t SizeType;
	typedef std::ptrdiff_t DifferenceType;

	CDynAllocator()
	~CDynAllocator();
	CDynAllocator& operator =(const CDynAllocator &rhs)
	size_t GetCapacity() const;
	explicit CDynAllocator(size_t Size);	
	DataType* GetPointer();
	void Resize(size_t Size);
};
*/

#endif	//	_ALLOCATOR_H_