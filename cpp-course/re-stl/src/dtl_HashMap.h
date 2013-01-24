#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include "dtl_Array.h"
#define DEFAULT_MAX_ENTRIES 8192

namespace dtl
{

template <typename KeyType>
class hash_map_key_traits
{
public:
	static unsigned int size(const KeyType &Key);
	static bool keys_compare(const KeyType &lhs, const KeyType &rhs);
};

template<typename KeyType, typename DataType>
class hash_map
{
public:
	IMPLEMENT_CONTAINER_ASSOC_TYPEDEFS

private:
	class CNode
	{
	public:
		int Next;

		CNode() : Next(-1), ValueHolder(NULL)
		{

		}

		void DestroyValue()
		{
			if (HasValue())
				delete ValueHolder, ValueHolder = NULL;
		}

		bool HasValue()
		{
			return ValueHolder != NULL;
		}

		value_type& GetValue()
		{
			if (!HasValue())
				throw CExceptionInvalidOperation();
			return ValueHolder->Value;
		}

		const value_type& GetValue() const
		{
			if (!HasValue())
				throw CExceptionInvalidOperation();
			return ValueHolder->Value;
		}

		void SetValue(const value_type &AValue)
		{
			if (!HasValue())
				ValueHolder = new CValueHolder();
			ValueHolder->Value = AValue;
		}

	private:
		class CValueHolder
		{
		public:
			value_type Value;
		};

		CValueHolder *ValueHolder;
	};

public:

		class iterator
		{
		private:
			hash_map *HashMap;
			CNode *Node;
			unsigned int Index;

			bool IsEnd() const
			{
				return Index > DEFAULT_MAX_ENTRIES;
			}

			bool IsInvalid() const
			{
				return (HashMap == NULL) || (Index < MAX_ENTRIES && Node == NULL);
			}

		public:

			iterator(hash_map *AHashMap = NULL) : HashMap(AHashMap), Index(0)
			{
				// ++(*this);
			}

			value_type& operator *()
			{
				if (!IsValid())
					throw CExceptionInvalidIteratorOperation();
				return Node->Value;
			}

			const value_type& operator *() const 
			{
				if (!IsValid())
					throw CExceptionInvalidIteratorOperation();
				return Node->Value;
			}

			iterator& operator++()
			{
				if (IsEnd() || IsInvalid())
					throw CExceptionInvalidIteratorOperation();

				if (Node->Next != NULL)
				{
					Node = Node->Next;
					return *this;
				}

				while(Index < MAX_ENTRIES && (Node = HashMap->HashTable[++Index]) == NULL);
				return *this;
			}

			iterator& operator--()
			{
				if (IsEnd())
					Index = Array->size() - 1;
				else
					(--Index);
				return *this;
			}

			iterator operator ++(int)
			{
				iterator TempIterator = *this;
				++*this;
				return TempIterator;
			}

			iterator operator --(int)
			{
				iterator TempIterator = *this;
				--*this;
				return TempIterator;
			}

			iterator operator +(int rhs) const
			{
				iterator TempIterator = *this;
				return TempIterator += rhs;
			}

			iterator operator -(int rhs) const
			{
				iterator TempIterator = *this;
				return TempIterator -= rhs;
			}

			iterator& operator +=(int rhs)
			{
				Index += rhs;
				return *this;
			}

			iterator& operator -=(int rhs)
			{
				Index -= rhs;
				return *this;
			}

			bool operator ==(const iterator &rhs) const
			{
				if (!IsValid() || !rhs.IsValid())
					throw CExceptionInvalidIteratorOperation();
				return Array == rhs.Array && (Index == rhs.Index || IsEnd() == rhs.IsEnd());
			}

			bool operator !=(const iterator &rhs) const
			{
				return !(*this == rhs);
			}

			int operator -(const iterator &rhs) const
			{
				if (!IsValid() || !rhs.IsValid() || Array != rhs.Array)
					throw CExceptionInvalidIteratorOperation();
				return Index - rhs.Index;
			}

		};

private:

	unsigned int ElementsNumber;
	dtl::vector<CNode> HashTable;

	unsigned int _Hash(const KeyType &Key)
	{
		unsigned int KeyLength = hash_map_key_traits<KeyType>::size(Key);
		unsigned int hash = 0;
		unsigned char byte = 0, *beginning = &Key;
		for (unsigned int i = 0; i < KeyLength; i++)
		{
			byte = *((unsigned char *)(beginning + i));
			hash += byte * (i+1);
		}
		hash %= MAX_ENTRIES;
		return hash;
	}

	static CNode** GetHashTableNode(const KeyType &Key)
	{
		int NodeIndex = 0;
		CNode **TempNode = NULL;
		NodeIndex = _Hash(hash_map_key_traits<KeyType>::size(Key), Key);
		if (HashTable[NodeIndex] == NULL)
			return &(Container->HashTable[NodeIndex]);
		TempNode = &(Container->HashTable[NodeIndex]);
		while((*TempNode) != NULL && (Container->KeyCompareFunction(Key, (*TempNode)->Key)))
			TempNode = &((*TempNode)->Next);
		return TempNode;
	}

	hash_map() : ElementsNumber(0)
	{
		
	}

	~hash_map()
	{
		ContainerT *container = (ContainerT *)handle;
		int i;
		if (container == NULL)
			return;
		for(i = 0; i < TABLE_MAX_ELEMENTS; i++)
		{
			HashTableNodeT *TempNode = container->HashTable[i];
			while(TempNode != NULL)
			{
				HashTableNodeT *PrevNode = TempNode;
				TempNode = TempNode->Next;
				free(PrevNode->Data);
				free(PrevNode->Key);
				free(PrevNode);
			}
		}
		free(container);
	}

	unsigned int size() const
	{
		return ElementsNumber;
	}



	DataType& operator [](const KeyType &Key)
	{
		

		iterator->Container = handle;
		iterator->Node = *GetHashTableNode(handle, key/*, PreviousNode*/);
		iterator->State = ITERATOR_STATE_NORMAL;
		if (iterator->Node == NULL)
			iterator->State = ITERATOR_STATE_PAST_REAR;
		return iterator;
	}

	iterator begin()
	{
		return iterator;
	}

	iterator end()
	{
		return iterator;
	}

	void insert(const KeyType &Key, const DataType &Value)
	{
// 		CNode **NewNode = NULL;
// 		NewNode = GetHashTableNode(container, key);
// 		if ((*NewNode) != NULL)
// 		{
// 			return (*NewNode)->Data;	
// 		}
// 		(*NewNode) = malloc(sizeof(CNode));
// 		if ((*NewNode) == NULL)
// 			return;
// 		(*NewNode)->Data = container->ValueCloneFunction(value);
// 		(*NewNode)->Key = container->KeyCloneFunction(key);
// 		(*NewNode)->Next = NULL;
		ElementsNumber++;
	}

	unsigned int erase(const KeyType &Key)
	{
		HashTableNodeT **NodeToDelete = NULL, *TempNode = NULL;
		NodeToDelete = GetHashTableNode(container, key);
		if (NodeToDelete == NULL || (*NodeToDelete) == NULL)
			return;
		TempNode = *NodeToDelete;
		(*NodeToDelete) = (*NodeToDelete)->Next;
		free(TempNode->Data);
		free(TempNode->Key);
		free(TempNode);
		ElementsNumber--;
	}

};

}	// namespace dtl
#endif	//	_HASH_MAP_H_