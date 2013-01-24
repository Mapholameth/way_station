#ifndef	_LIST_H_
#define	_LIST_H_

#include <stdexcept>
#include "dtl_Allocator.h"
#include "dtl_Utility.h"

class CListBase
{
private:
	size_t ElementsNumber;

protected:
	class CNodeBase
	{
	public:
		CNodeBase *Next;
		CNodeBase *Prev;		

		CNodeBase (CNodeBase* ANext = NULL, CNodeBase *APrev = NULL) : Next(ANext), Prev(APrev) {}
	};

	class CIteratorBase
	{
	public:
		CListBase *List;
		CNodeBase *Node;

		CIteratorBase(CListBase *AList) : List(AList), Node(AList->Start.Next){}
		void _NextNode();
		void _PrevNode();

	public:
		bool Ok() const;
	};

	CNodeBase Start;

	CListBase() : Start(&Start, &Start), ElementsNumber(0){}	
	

public:
	void clear();
	void IncSize();
	void DecSize();
	virtual void DeleteRear() = 0;
	bool empty() const;
	size_t size() const;
};

template <typename DataType>
class CList : public CListBase
{
private:
	class CNode : public CListBase::CNodeBase
	{
	public:
		DataType Data;
		CNode (){}
		CNode (const DataType& AData, CNodeBase *ANext, CNodeBase *APrev) : Data(AData), CNodeBase(ANext, APrev){}
	};	

	void CopyList(const CList &AList)
	{
		clear();
		for(CNodeBase *TempNode = AList.Start.Next; TempNode != &AList.Start; TempNode = TempNode->Next)
		{
			push_back((static_cast<CNode*>(TempNode))->Data);
		}
	}

public:	
	template <typename BaseIterator>
	class CCommonIterator : public BaseIterator
	{
	public:
		CCommonIterator(CList *AList) : BaseIterator(AList){}
		DataType& operator *()
		{
			return static_cast<CNode*>(Node)->Data;
		}
		CCommonIterator& operator++()
		{
			_NextNode();
			return *this;
		}
		CCommonIterator& operator--()
		{
			_PrevNode();
			return *this;
		}
		CCommonIterator operator ++(int)
		{
			CCommonIterator TempIterator = *this;
			++*this;
			return TempIterator;
		}
		CCommonIterator operator --(int)
		{
			CCommonIterator TempIterator = *this;
			--*this;
			return TempIterator;
		}
		CCommonIterator operator +(int Operand) const
		{
			CCommonIterator TempIterator = *this;
			return TempIterator += Operand;
		}
		CCommonIterator operator -(int Operand) const
		{
			CCommonIterator TempIterator = *this;
			return TempIterator -= Operand;
		}
		CCommonIterator& operator +=(int Operand)
		{
			int increment = Operand < 0 ? -Operand : Operand;
			while (increment)
			{
				Node = Operand < 0 ? Node->Prev : Node->Next;
				--increment;
			}
			return *this;
		}
		CCommonIterator& operator -=(int Operand)
		{
			int increment = Operand < 0 ? -Operand : Operand;
			while (increment)
			{
				Node = Operand < 0 ? Node->Next : Node->Prev;
				--increment;
			}
			return *this;
		}
		bool operator ==(const CCommonIterator &I) const
		{
			return List == I.List && Node == I.Node;
		}
		bool operator !=(const CCommonIterator &I) const
		{
			return !(*this == I);
		}
		bool operator >(const CCommonIterator &I) const;
		bool operator <(const CCommonIterator &I) const;
		bool operator >=(const CCommonIterator &I) const;
		bool operator <=(const CCommonIterator &I) const;		
	};

	class CIntermediateIterator : public CListBase::CIteratorBase
	{
	public:
		CIntermediateIterator(CList *AList) : CIteratorBase(AList){}
		void InsertBefore(const DataType &AElement)
		{
			Node->Prev = Node->Prev->Next = new CNode(AElement, Node, Node->Prev);
			++List->ElementsNumber;
		}
		void InsertAfter(const DataType &AElement)
		{
			Node->Next = Node->Next->Prev = new CNode(AElement, Node->Next, Node);
			List->IncSize();			
		}
		void Remove()
		{
			if (List->empty())
				throw CDeleteOnEmptyException();
			if (!Ok())
				throw CExceptionDeleteStartNode();
			CNodeBase *TempNode = Node;
			Node->Next->Prev = Node->Prev;
			Node->Prev->Next = Node->Next;
			Node = Node->Next;
			delete TempNode;
			List->DecSize();
		}
	};

	IMPLEMENT_CONTAINER_SEQ_TYPEDEFS
	typedef unsigned int KeyType;
	typedef CCommonIterator<CListBase::CIteratorBase> CConstIterator;
	typedef CCommonIterator<CIntermediateIterator> iterator;

	CList()
	{

	}

	CList(const CList &AList)
	{
		CopyList(AList);
	}

	~CList()
	{
		clear();
	}

	CList& operator =(CList &Operand)
	{
		if (this == &Operand)
			return *this;
		clear();
		for(CList<DataType>::iterator i(&Operand); i.Ok(); i++)
			push_back(*i);
		return *this;
	}

	iterator begin()
	{
		return iterator(this);
	}

	CConstIterator begin() const
	{
		return CConstIterator(const_cast<CList *>(this));
	}

	iterator End()
	{
		iterator i(this);
		i.Node = Start.Prev;
		return i;
	}

	CConstIterator End() const
	{
		CConstIterator i(const_cast<CList *>(this));
		i.Node = Start.Prev;
		return i;
	}

	DataType& Front() const
	{
		return *begin();
	}

	DataType& Rear() const
	{
		return *End();
	}	

	DataType& operator [](unsigned int Index)
	{
		if (Index >= size())
			throw CExceptionIndexOutOfBounds();
		CNodeBase *TempNode = Start.Next;
		for(unsigned int i = 0; i != Index; i++, TempNode = TempNode->Next);
		return static_cast<CNode *>(TempNode)->Data;			
	}

	void AddFront(const DataType &AElement)
	{
		Start.Next = Start.Next->Prev = new CNode(AElement, Start.Next, &Start);
		IncSize();
	}

	void push_back(const DataType &AElement)
	{
		Start.Prev = Start.Prev->Next = new CNode(AElement, &Start, Start.Prev);
		IncSize();
	}

	void DeleteRear()
	{
		if (empty())
			throw CExceptionDeleteFromEmpty();
		CNodeBase *TempNode = Start.Prev;
		Start.Prev = Start.Prev->Prev;
		Start.Prev->Next = &Start;
		delete TempNode;
		DecSize();
	}

	void DeleteFront()
	{
		if (empty())
			throw CExceptionDeleteFromEmpty();
		CNodeBase *TempNode = Start.Next;
		Start.Next = Start.Next->Next;
		Start.Next->Prev = &Start;
		delete TempNode;
		DecSize();
	}

	DataType Pop()
	{
		DataType temp = Rear();
		DeleteRear();
		return temp;
	}

	void Push(const DataType &AElement)
	{
		push_back(AElement);
	}
};

template <typename DataType>
class CSafeList : public CList<DataType>
{
private:
	size_t GenerationNumber;

public:
	class CSafeIterator : public CList<DataType>::iterator
	{
	private:
		size_t GenerationNumber;
	public:
		CSafeIterator(CSafeList *AList) : CList<DataType>::iterator(AList), GenerationNumber(0){}

	};

	CSafeList() : GenerationNumber(0)
	{
	}
	
};

#endif	//	_CLIST_H_