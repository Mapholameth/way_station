#ifndef _SKIP_LIST_H_
#define _SKIP_LIST_H_

#include <math.h>
#include "dtl_Array.h"
using dtl::vector;

#ifdef _DEBUG
	#include <cassert>
#endif	//	_DEBUG

#define p_1 4
const float p = 1.0f / p_1;	// Probability factor for node on i-th level to appear on (i+1)-th level
const unsigned int SKIP_LIST_MAX_ENTRIES = 65536;	// Predefined expected maximum number of entries per SkipList
const unsigned int SKIP_LIST_MAX_LEVEL = StaticLog<SKIP_LIST_MAX_ENTRIES, p_1>::value;	// Compile-time calculated max level count

/*
	@todo: Хранить текущее значение максимальной высоты среди всех узлов скип листа.
			И потом ещё использовать чёто там для оптимизации чего-то там. keyword: Width;
*/

template<typename KeyType, typename DataType>	// Data & Key
class CSkipList
{
public:
	static DataType DefaultData;
	static KeyType DefaultKey;

private:
	class CNode
	{
	private:
		CNode();

	public:
		DataType Data;
		KeyType Key;
		vector<CNode*, CDynAllocator<CNode*, 1, 1> > Next;

		CNode(unsigned int ALevels, const KeyType &AKey = DefaultKey, const DataType &AData = DefaultData);
	};

	size_t ElementsNumber;
	CNode Start;

	/**
	*	Конструирует узел из всех ближайших предшествующих узлов
	*	каждого уровня для того значения ключа, которое мы ищем.
	*/
	CNode FindInsertionPoint(const KeyType &Key, unsigned int NewNodeLevels);
	unsigned int RandomLevel(); 

public:
	class iterator
	{
	private:
		CSkipList *SkipList;
		CNode *Node;
		iterator();
	public:
		iterator(CSkipList *ASkipList);

		bool Ok();
		DataType& operator *();
		iterator& operator --();
		iterator& operator ++();
		iterator& operator --(int);
		iterator& operator ++(int);
		iterator& operator +=(int Operand);		
		//iterator& operator -=(int Operand)		{	return *this;		}
		iterator operator +(int Operand) const;
		//iterator operator -(int Operand) const{}
	};

	IMPLEMENT_CONTAINER_ASSOC_TYPEDEFS

	CSkipList();
	CSkipList(const CSkipList &OtherSkipList);
	~CSkipList();

	bool empty() const;
	void Add(const DataType &NewElement, const KeyType &Key);
	void Delete(const KeyType &Key);
	void clear();
	size_t size() const;
	iterator& begin();
	iterator& End();
	void DebugPrint();
	DataType& operator [](const KeyType &Key);
};

template<typename KeyType, typename DataType>
DataType CSkipList<KeyType, DataType>::DefaultData = DataType();
template<typename KeyType, typename DataType>
KeyType CSkipList<KeyType, DataType>::DefaultKey = KeyType();

//////////////////////////////////////////////////////////////////////////
//	CSkipList::CNode

template <typename KeyType, typename DataType>
CSkipList<KeyType, DataType>::CNode::CNode()
{
	assert(false);
}

template <typename KeyType, typename DataType>
CSkipList<KeyType, DataType>::CNode::CNode(unsigned int ALevels, const KeyType &AKey, const DataType &AData) : Data(AData),
	Key(AKey), Next(ALevels)
{

}

//////////////////////////////////////////////////////////////////////////
// CSkipList::iterator

template <typename KeyType, typename DataType>
CSkipList<KeyType, DataType>::iterator::iterator()
{
	assert(false);
}

template <typename KeyType, typename DataType>
CSkipList<KeyType, DataType>::iterator::iterator(CSkipList *ASkipList) : SkipList(ASkipList), Node(SkipList->Start.Next[0])
{

}

template<typename KeyType, typename DataType>
bool CSkipList<KeyType, DataType>::iterator::Ok()
{
	return Node != &SkipList->Start;
}

template<typename KeyType, typename DataType>
DataType& CSkipList<KeyType, DataType>::iterator::operator*()
{
	if (!Ok())
		throw std::bad_exception();
	return Node->Data;
}

template<typename KeyType, typename DataType>
typename CSkipList<KeyType, DataType>::iterator& CSkipList<KeyType, DataType>::iterator::operator --()
{
	CNode TempNode = FindInsertionPoint(Node->Key, SKIP_LIST_MAX_LEVEL);
	Node = &TempNode;
	return TempNode.Data;
}

template<typename KeyType, typename DataType>
typename CSkipList<KeyType, DataType>::iterator& CSkipList<KeyType, DataType>::iterator::operator ++()
{
	assert(Ok() && Node != NULL && SkipList != NULL);
	Node = Node->Next[0];
	return *this;
}

template<typename KeyType, typename DataType>
typename CSkipList<KeyType, DataType>::iterator& CSkipList<KeyType, DataType>::iterator::operator--(int)
{
	iterator TempIterator = *this;
	--*this;
	return TempIterator;
}

template<typename KeyType, typename DataType>
typename CSkipList<KeyType, DataType>::iterator& CSkipList<KeyType, DataType>::iterator::operator++(int)
{
	iterator TempIterator(*this);
	++*this;
	return TempIterator;
}

template<typename KeyType, typename DataType>
typename CSkipList<KeyType, DataType>::iterator& CSkipList<KeyType, DataType>::iterator::operator+=(int Operand)
{
	assert(Operand >= 0);
	while (Operand--)
		Node = Node->Next[0];
	return *this;
}

template<typename KeyType, typename DataType>
typename CSkipList<KeyType, DataType>::iterator CSkipList<KeyType, DataType>::iterator::operator+(int Operand) const
{
	iterator TempIterator(*this);
	while (Operand--)
		TempIterator.Node = TempIterator.Node->Next[0];
	return TempIterator;
}

//////////////////////////////////////////////////////////////////////////
//	CSkipList

template<typename KeyType, typename DataType>
unsigned int CSkipList<KeyType, DataType>::RandomLevel()
{
	unsigned int lvl = static_cast<unsigned int>(log(frand()) / log(1.0 - p));
	return lvl < SKIP_LIST_MAX_LEVEL ? lvl + 1 : SKIP_LIST_MAX_LEVEL;
}


template<typename KeyType, typename DataType>
typename CSkipList<KeyType, DataType>::CNode CSkipList<KeyType, DataType>::FindInsertionPoint(const KeyType &Key, unsigned int NewNodeLevels)
{
	CNode BackLookNode(NewNodeLevels);
	unsigned int Level = Start.Next.size();
	CNode *TempNode = &Start;
	while (Level--)
	{
		while (TempNode->Next[Level]->Key < Key && TempNode->Next[Level] != &Start)
			TempNode = TempNode->Next[Level];
		if (Level + 1 <= NewNodeLevels)
			BackLookNode.Next[Level] = TempNode;
	}		
	return BackLookNode;
}



template<typename KeyType, typename DataType>
CSkipList<KeyType, DataType>::CSkipList() : ElementsNumber(), Start(SKIP_LIST_MAX_LEVEL)
{
	for(int i = 0; i < SKIP_LIST_MAX_LEVEL; i++)
		Start.Next[i] = &Start;
}

// CSkipList(const CSkipList &OtherSkipList) : ElementsNumber(), Start(SKIP_LIST_MAX_LEVEL)
// {
// 	for(CSkipList::iterator i = OtherSkipList.begin(); i.Ok(); ++i)
// 		
// }

template<typename KeyType, typename DataType>
CSkipList<KeyType, DataType>::~CSkipList()
{
	clear();
}

template<typename KeyType, typename DataType>
bool CSkipList<KeyType, DataType>::empty() const
{
	return ElementsNumber == 0;
}

template<typename KeyType, typename DataType>
void CSkipList<KeyType, DataType>::Add(const DataType &NewElement, const KeyType &Key)
{
	operator[](Key) = NewElement;
}

template<typename KeyType, typename DataType>
void CSkipList<KeyType, DataType>::Delete(const KeyType &Key)
{
	CNode BackLookNode = FindInsertionPoint(Key, SKIP_LIST_MAX_LEVEL);
	if (BackLookNode.Next[0]->Next[0]->Key != Key || BackLookNode.Next[0]->Next[0] == &Start)
		return; // throw 
	CNode *Temp = Back.Next[0]->Next[0];
	unsigned int Level = Temp->Next.size();
	while (Level--)
	{
		BackLookNode.Next[Level]->Next[Level] = BackLookNode.Next[Level]->Next[Level]->Next[Level];
	}
	ElementsNumber--;
}

template<typename KeyType, typename DataType>
void CSkipList<KeyType, DataType>::clear()
{
	while(ElementsNumber--)
		;//DeleteRear();
}

template<typename KeyType, typename DataType>
size_t CSkipList<KeyType, DataType>::size() const
{
	return ElementsNumber;
}

template<typename KeyType, typename DataType>
typename CSkipList<KeyType, DataType>::iterator& CSkipList<KeyType, DataType>::begin()
{
	return iterator(this);
}

template<typename KeyType, typename DataType>
typename CSkipList<KeyType, DataType>::iterator& CSkipList<KeyType, DataType>::End()
{
	assert(false);
	return iterator;
}

template<typename KeyType, typename DataType>
void CSkipList<KeyType, DataType>::DebugPrint()
{
	unsigned int Level = SKIP_LIST_MAX_LEVEL - 1;
	CNode *Node = NULL;
	do
	{
		Node = Start.Next[Level];
		while(Node != &Start)
		{
			cout << Node->Key << " -> ";
			Node = Node->Next[Level];
		}
		cout << endl;			
	}while (Level--);
}

template<typename KeyType, typename DataType>
DataType& CSkipList<KeyType, DataType>::operator[](const KeyType &Key)
{
	unsigned int NewNodeLevels = RandomLevel();		
	CNode BackLookNode = FindInsertionPoint(Key, NewNodeLevels);		
	if (BackLookNode.Next[0]->Next[0]->Key == Key && BackLookNode.Next[0]->Next[0] != &Start)
		return BackLookNode.Next[0]->Next[0]->Data;
	CNode *NewNode = new CNode(NewNodeLevels, Key);
	while (NewNodeLevels--)
	{
		NewNode->Next[NewNodeLevels] = BackLookNode.Next[NewNodeLevels]->Next[NewNodeLevels];
		BackLookNode.Next[NewNodeLevels]->Next[NewNodeLevels] = NewNode;
	}
	ElementsNumber++;
	return NewNode->Data;
}

#endif // _SKIP_LIST_H_