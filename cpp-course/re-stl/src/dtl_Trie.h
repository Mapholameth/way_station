#ifndef _TRIE_H_
#define _TRIE_H_

#include "dtl_Exception.h"
#include "dtl_Array.h"
using dtl::vector;

/**
*	CTrieKeyTraits have to know how to:
*		- initialize the key
*		- make Key from array of unsigned integers
*		- destroy the key
*	This is made because we have to use different key types particularly in
*	iterator when combining indices to make the key.
*/

template <typename KeyType>
class CTrieKeyTraits;

/**
*	CTrieKeyMapping	in general describes mapping of KeyElement to integral index and vice versa:
*		- MapFromKey()
*		- MapToKey()
*	Such mapping used in order to minimize memory consumption by CTrie.
*	i.e. if user knows alphabet he'll use, he can make compact mapping in this way:
*		alphabet			indices
*	{123, 456, 789) <-> {0, 1, 2}
*	@todo: Will it be efficient to sort mapping by frequency of use of alphabet elements
*		i.e. if 'a' used more than 'q' then index of 'a' should be less, then index of 'a'
*/

template<typename KeyType, unsigned int OffsetLeft = 32>
class CTrieKeyMappingOffset;

template<typename KeyType>
class CTrieKeyMappingSimple;

/**
*	CTrie;
*	D: Data type
*	K: Key type have to:
*		- support "<integer type> CKeyType::operator [] (unsigned int);"
*		- be a 0 terminating sequence
*	@todo: think of making automated mapping from something like 
		{613, 435, 245, 587) to {0, 1, 2, 3, 4} in some uniform way by (O(1))
*		ot at least from 97 98 99 to 0 1 2
*	@todo: think about optimizations, like burst tries, for example
*	@todo: think about policy based trie
*	@todo: change trie(and all other containers in future) interface to stl-compatible
*/

template 
<
	typename KeyType,
	typename DataType,
	typename ElementMapping = CTrieKeyMappingOffset<KeyType, 32>
>	
class CTrie
{
private:
	class CNode
	{
	public:
		vector<CNode*, CDynAllocator<CNode*, 2, 1>> Children;	// Allocator used here should be not-implemented-yet-pool-allocator
		// This ^ consumes such large amount of memory, that inserting two more 4-byte fields wont hurt anyone.
		unsigned int Skip;	// Used to skip first Skip number of children which are invalid (NULL)
		// CNode* Parent; - I'll check if this will make my life with \
			iterators easier, and add this field then. But it's unlikely to happen
		unsigned int Index;	// Node index in parent Node->Children
		CNode() : DataHolder(NULL), Skip(0)
		{

		}

		~CNode()
		{
			clear();
		}

		void clear()
		{
			for(unsigned int i = Skip; i < Children.size(); i++)
				if (Children[i] != NULL)
				{
					Children[i]->clear();
					delete Children[i];
				}
			Children.clear();
			if (HasData())
				DestroyData();
		}

		void DestroyData()
		{
			if (HasData())
				delete DataHolder, DataHolder = NULL;
		}

		bool HasData()
		{
			return DataHolder != NULL;
		}

		DataType& GetData()
		{
			if (!HasData())
				throw CExceptionInvalidOperation();
			return DataHolder->Data;
		}

		const DataType& GetData() const
		{
			if (!HasData())
				throw CExceptionInvalidOperation();
			return DataHolder->Data;
		}

		void SetData(const DataType &AData)
		{
			if (!HasData())
				DataHolder = new CDataHolder();
			DataHolder->Data = AData;
		}

	private:
		class CDataHolder
		{
		public:
			DataType Data;
		};

		CDataHolder *DataHolder;
	};

	size_t ElementsNumber;
	CNode Root;

public:
	IMPLEMENT_CONTAINER_ASSOC_TYPEDEFS
	class iterator
	{
		friend CTrie;

	private:
		CTrie *Trie;
		vector<CNode*> Path;
		vector<unsigned int> KeyElements;
		KeyType CurrentKey;	// Storing it here, cause if for example KeyType is char* we'll have to delete KeyInstance

		void ShiftPosition(int Amount)
		{
			if (Amount == 0)
				return;
			int AbsAmount = std::abs(Amount);
			while(AbsAmount--)
			{
				Amount > 0 ? ++*this : --*this;
			}
		}

		unsigned int FindNextValidIndex(unsigned int From = 0)
		{
			unsigned int i = From;
			CNode *Node = Path.Rear();
			for(; i < Node->Children.size() && Node->Children[i] == NULL; i++);
			return i;
		}

		unsigned int FindPrevValidIndex(unsigned int From)
		{
			unsigned int i = From;
			CNode *Node = Path.Rear();
			for(; i >= Node->Skip && Node->Children[i] == NULL; i--);
			return i;
		}

		void FindNextEntry(CNode *Node = NULL)
		{
			if (Node == NULL)
				return;
			Path.push_back(Node);
			KeyElements.push_back(Node->Index);
			if (Node->HasData())
				return;
			unsigned int i = FindNextValidIndex(Node->Skip);
			FindNextEntry(Node->Children[i]);
		}

		void FindPrevEntry(CNode *Node = NULL)
		{
			if (Node == NULL)
				return;
			Path.push_back(Node);
			KeyElements.push_back(Node->Index);
			if (Node->HasData() && Node->Children.size() == 0)
				return;
			unsigned int i = FindPrevValidIndex(Node->Children.size() - 1);
			FindPrevEntry(Node->Children[i]);
		}

		void MoveToFront()
		{
			CNode *Root = &Trie->Root;
			Path.push_back(Root);
			if (!Root->HasData())
				FindNextEntry(Root->Children[Root->Skip]);
		}

		void MoveToRear()
		{
			CNode *Root = &Trie->Root;
			Path.push_back(Root);
			FindPrevEntry(Root->Children[Root->Children.size() - 1]);
		}

	public:
		iterator(CTrie *ATrie = NULL) : Trie(ATrie)
		{
			if (Trie == NULL)
				return;
			CTrieKeyTraits<KeyType>::InitKey(CurrentKey);
			MoveToFront();
		}

		~iterator()
		{
			CTrieKeyTraits<KeyType>::DestroyKey(CurrentKey);
		}

		bool Ok()
		{
			return Path.size() != 0 && Path.Rear()->HasData();
		}

		value_type operator *()
		{
			// @todo: this function may have performance leak due to \
				iterations below. Maybe redesign Key output from iterator.
			if (!Ok())
				throw CExceptionInvalidIteratorOperation();
			vector<typename CTrieKeyTraits<KeyType>::ElementType> MappedKeyElements;
			for(unsigned int i = 0; i < KeyElements.size(); i++)
				MappedKeyElements[i] = ElementMapping::MapToKey(KeyElements[i]);
			CTrieKeyTraits<KeyType>::KeyFromArray(CurrentKey, MappedKeyElements);
			return std::make_pair(CurrentKey,  Path.Rear()->GetData());
		}

		iterator& operator --()
		{
			if (Path.size() == 0)
			{
				MoveToRear();
				return *this;
			}
			if (!Ok())
				throw CExceptionInvalidIteratorOperation();
			Path.DeleteRear();
			CNode* TempNode = Path.Rear();
			if (TempNode->HasData())
			{
				KeyElements.DeleteRear();
				return *this;
			}
			
			bool GoingBack = true;
			while (TempNode != NULL)
			{
				unsigned int i = TempNode->Children.size() - 1;
				if (GoingBack && KeyElements.size() > 0)
				{
					i = KeyElements.Rear() - 1;
					KeyElements.DeleteRear();
				}
				for(; i < TempNode->Children.size() && i >= TempNode->Skip && TempNode->Children[i] == NULL; i--);
				TempNode = TempNode->Children[i];
				if (TempNode == NULL)
				{
					if (Path.Rear()->HasData())
					{
						return *this;
					}
					Path.DeleteRear();
					if (Path.size() == 0)
						return *this;
					TempNode = Path.Rear();
					GoingBack = true;
					continue;
				}
				Path.push_back(TempNode);
				GoingBack = false;
				KeyElements.push_back(i);
				if (TempNode->HasData() && TempNode->Children.size() == 0)
					break;
			}
			return *this;
		}

		iterator& operator ++()
		{
			if (Path.size() == 0)
			{
				MoveToFront(); 
				return *this;
			}
			if (!Ok())
				throw CExceptionInvalidIteratorOperation();

			CNode* TempNode = Path.Rear();
			bool GoingBack = false;
			while (TempNode != NULL)
			{
				unsigned int i = TempNode->Skip;
				if (GoingBack && KeyElements.size() > 0)
				{
					i = KeyElements.Rear() + 1;
					KeyElements.DeleteRear();
				}
				for(; i < TempNode->Children.size() && TempNode->Children[i] == NULL; i++);
				if (i >= TempNode->Children.size())
				{
					Path.DeleteRear();
					if (Path.size() == 0)
						return *this;
					TempNode = Path.Rear();
					GoingBack = true;
					continue;
				}
				else
					TempNode = TempNode->Children[i];
				Path.push_back(TempNode);
				GoingBack = false;
				KeyElements.push_back(i);
				if (TempNode->HasData())
					return *this;
			}
			return *this;
		}

		iterator operator --(int)
		{
			iterator TempIterator = *this;
			--*this;
			return TempIterator;
		}

		iterator operator ++(int)
		{
			iterator TempIterator = *this;
			++*this;
			return TempIterator;
		}

		iterator& operator +=(int Operand)
		{
			ShiftPosition(Operand);
			return *this;
		}
		
		iterator& operator -=(int Operand)
		{
			ShiftPosition(-Operand);
			return *this;
		}

		iterator operator +(int Operand) const
		{
			iterator TempIterator = *this;
			return TempIterator += Operand;
		}
		iterator operator -(int Operand) const
		{
			iterator TempIterator = *this;
			return TempIterator -= Operand;
		}
	};

	CTrie() : ElementsNumber(0)
	{

	}
	~CTrie()
	{
		// Root destructor automatically invoked and whole tree being destroyed
	};

	void insert(const value_type &NewElement)	// should return iterator
	{
		(*this)[NewElement.first] = NewElement.second;
	}

	unsigned int erase(const KeyType &Key)	// Returns number of elements erased
	{
		
	}

	void clear()
	{
		Root.clear();
	}

	size_t size() const
	{
		return ElementsNumber;
	}

	bool empty() const
	{
		return ElementsNumber == 0;
	}

	iterator begin()
	{
		return iterator(this);
	}

	iterator end()
	{
		iterator Iterator;
		Iterator.MoveToRear();
		return Iterator;
	}

	bool find(const KeyType& Key)
	{
		unsigned int i = 0, ki = 0;
		CNode *TempNode = &Root;
		while(Key[i] != 0)
		{
			ki = ElementMapping::MapFromKey(Key[i]);
			if (TempNode->Children[ki] == NULL)
				return false;
			TempNode = TempNode->Children[ki];
			i++;
		}
		return true;
	}

	DataType& operator [](const KeyType &Key)
	{
		unsigned int i = 0, ki = 0;
		CNode *TempNode = &Root;
		while(Key[i] != 0)
		{
			ki = ElementMapping::MapFromKey(Key[i]);
			if (TempNode->Children[ki] == NULL)
				TempNode->Children[ki] = new CNode();
			TempNode->Skip = std::min(TempNode->Skip - 1, ki - 1) + 1;
			TempNode = TempNode->Children[ki];
			TempNode->Index = ki;
			i++;
		}
 		if (!TempNode->HasData())
 			TempNode->SetData(DataType()), ElementsNumber++;;
		return TempNode->GetData();
	}

	const DataType& operator[](const KeyType &Key) const
	{
		unsigned int i = 0, ki = 0;
		CNode *TempNode = &Root;
		while(Key[i] != 0)
		{
			ki = ElementMapping::MapFromKey(Key[i]);
			if (TempNode->Children[ki] == NULL)
				throw(CExceptionIndexOutOfBounds());
			TempNode = TempNode->Children[ki];
			i++;
		}
		if (!TempNode->HasData())
			throw(CExceptionIndexOutOfBounds());
		return TempNode->GetData();
	}
};

//////////////////////////////////////////////////////////////////////////
// KeyTypeTraits specializations

template <>
class CTrieKeyTraits<char *>
{
public:
	typedef char * KeyType;
	typedef char ElementType;

	static void InitKey(KeyType &Key)
	{
		Key = NULL;
	}

	static void KeyFromArray(KeyType &Key, vector<ElementType> &Array)
	{
		if (Key == NULL)
			Key = new char [Array.size() + 1];
		else if (strlen(Key) < Array.size())
		{
			delete [] Key;
			Key = new char [Array.size() + 1];
		}
		for(unsigned int i = 0; i < Array.size(); i++)
			Key[i] = static_cast<char>(Array[i]);
		Key[Array.size()] = 0;
	}

	static void DestroyKey(KeyType &Key)
	{
		delete Key;
	}
};

template <>
class CTrieKeyTraits<std::string>
{
public:
	typedef std::string KeyType;
	typedef char ElementType;

	static void InitKey(KeyType &Key)
	{
		Key = "";
	}

	static void KeyFromArray(KeyType &Key, vector<ElementType> &Array)
	{
		Key.clear();
		for(unsigned int i = 0; i < Array.size(); i++)
			Key += Array[i];
	}

	static void DestroyKey(KeyType &Key)
	{

	}
};

//////////////////////////////////////////////////////////////////////////
// sample KeyMap implementations

template<typename KeyType>
class CTrieKeyMappingSimple
{
public:
	static inline unsigned int MapFromKey(typename CTrieKeyTraits<KeyType>::ElementType Element)
	{
		return static_cast<unsigned char>(Element);
	}

	static inline typename CTrieKeyTraits<KeyType>::ElementType MapToKey(unsigned int Element)
	{
		return Element;
	}
};

template<typename KeyType, unsigned int OffsetLeft = 32>
class CTrieKeyMappingOffset
{
public:
	static inline unsigned int MapFromKey(typename CTrieKeyTraits<KeyType>::ElementType Element)
	{
		return static_cast<unsigned char>(Element) - OffsetLeft;
	}

	static inline typename CTrieKeyTraits<KeyType>::ElementType MapToKey(unsigned int Element)
	{
		return Element + OffsetLeft;
	}
};

#endif // _TRIE_H_
