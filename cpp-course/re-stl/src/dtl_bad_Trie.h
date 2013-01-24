#ifndef _BAD_TRIE_H_
#define _BAD_TRIE_H_

#include "dtl_Exception.h"
#ifdef _DEBUG
	#include <cassert>
#endif	//	_DEBUG

template<typename KeyType, typename DataType>	// i.e. Data type & Key type
class CTrieBad
{
private:
	class CNodeBase
	{
	public:
		KeyType Key;
		CNodeBase *Next;
		CNodeBase *Child;

		CNodeBase() : Key(), Next(NULL), Child(NULL)
		{
		}
	};
	class CNode : public CNodeBase
	{
	public:
		DataType Data;
	};

	size_t ElementsNumber;
	CNodeBase *Root;

	CNode *InsertRestOfTheKey(CNodeBase * Node, const KeyType &Key, unsigned int From)
	{
		CNodeBase *TempNode = Node;
		while (Key.length() > From && static_cast<unsigned char>(Key[From]) != 0)
		{
			TempNode->Child = new CNodeBase();
			TempNode->Child->Key = Key[From];
			TempNode = TempNode->Child;
			TempNode->Next = TempNode;
			From++;
		}
		TempNode->Child = new CNode();
		TempNode->Child->Key = "";
		TempNode->Child->Next = TempNode->Child;
		ElementsNumber++;
		return static_cast<CNode *>(TempNode->Child);
	}

public:
	class iterator
	{
		friend CTrieBad;
	private:
		CTrieBad *Map;
		CNode *Node;
	public:
		iterator(){}
		bool Ok(){}
		DataType& operator *()
		{
			if (!Ok())
				throw "";
		}

		iterator& operator --()
		{
		}

		iterator& operator ++()
		{
		}

		iterator& operator --(int)
		{
			iterator TempIterator = *this;
			--*this;
			return TempIterator;
		}

		iterator& operator ++(int)
		{
			iterator TempIterator = *this;
			++*this;
			return TempIterator;
		}

		iterator& operator +=(int Operand)
		{
			return *this;
		}
		
		iterator& operator -=(int Operand)
		{

			return *this;
		}

		iterator operator +(int Operand) const
		{}
		iterator operator -(int Operand) const
		{}
	};

	CTrieBad(int stub = 0) : ElementsNumber(0), Root(NULL)
	{

	}

	~CTrieBad()
	{
		clear();
	}

	void Add(const DataType &NewElement, const KeyType &Key)
	{
	}

	void Delete(const KeyType &Key)
	{
	}

	void clear()
	{

	}

	size_t Size()
	{
		return ElementsNumber;
	}

	iterator& begin()
	{
		return iterator(this);
	}

	iterator& End()
	{
		return iterator;
	}

	DataType& operator [](const KeyType &Key)
	{
		CNodeBase *TempNode = Root;
		if (TempNode == NULL)
		{
			TempNode = new CNodeBase();
			TempNode->Key = Key[0];
			TempNode->Next = TempNode;
			Root = TempNode;
			return InsertRestOfTheKey(TempNode, Key, 1)->Data;
		}
		unsigned int i = 0;
		CNodeBase *PivotNode = NULL;
		for(;;)
		{			
			CNodeBase *PrevNode = TempNode;
			for(PivotNode = TempNode;
				static_cast<unsigned char>(PivotNode->Key[0]) < static_cast<unsigned char>(Key[i]) && PivotNode->Next != TempNode;
				PrevNode = PivotNode, PivotNode = PivotNode->Next)
				;
			if (PivotNode->Key[0] == Key[i])
			{
				if (PivotNode->Child == NULL)
					return static_cast<CNode *>(PivotNode)->Data;
				i++;
				TempNode = PivotNode->Child;
				continue;
			}
			else
			{		
				if (static_cast<unsigned char>(PivotNode->Key[0]) < static_cast<unsigned char>(Key[i]) && PivotNode->Next == TempNode)
				{
					PrevNode = PivotNode;
					PivotNode = TempNode;
				}
				CNodeBase *NewNode = new CNodeBase();
				NewNode->Key += Key[i++];
				/*NewNode->Key += '0';*/
				PrevNode->Next = NewNode;
				NewNode->Next = PivotNode;
				return InsertRestOfTheKey(NewNode, Key, i)->Data;
			}
			//return TempNode->D
		}
	}
};

#endif // _BAD_TRIE_H_