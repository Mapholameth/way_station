#ifndef _DTL_KDTREE_H_
#define _DTL_KDTREE_H_

//#include "dtl_Array.h"
//#include "2de_Math.h"
#include "2de_Vector2.h"
#include <utility>

/**
*	CKDTree - k-dimensional binary space partitioning tree.
*	DataType - anything you want to store in tree nodes;
*	KeyType - required to:
*		- have [] operator in order to access it's k elements. 
*		- have LengthSquared() function as distance metric
*		- overloaded "operator -"
*/
template <typename KeyType, typename DataType>
class CKDTree
{
public:
		typedef std::pair<KeyType, DataType> ValueTypeNonConst;
private:

	class CNode
	{
	public:
		KeyType Key;
		DataType Data;
		unsigned int Axis;	// Splitting hyperplane is orthogonal to (0, 0, ..., Axis, ..., 0, 0) Axis, at this node
		CNode *Left;
		CNode *Right;
		bool Deleted;

		CNode(KeyType APoint = KeyType(), DataType AData = DataType(), CNode *ALeft = NULL,
			CNode *ARight = NULL, unsigned int ASplittingDimension = 0) : Deleted(false), Key(APoint),
			Data(AData), Left(ALeft), Right(ARight), Axis(ASplittingDimension)
		{}

		void Clear()
		{
			if (Left != NULL)
				Left->Clear();
			delete Left, Left = NULL;
			if (Right != NULL)
				Right->Clear();
			delete Right, Right = NULL;
		}

		void CollectPoints(vector<ValueTypeNonConst> &Values)
		{
			if (!Deleted)
				Values.push_back(std::make_pair(Key, Data));
			if (Left != NULL)
				Left->CollectPoints(Values);
			if (Right != NULL)
				Right->CollectPoints(Values);
		}

		CNode* FindInsetionPoint(const KeyType &AKey)
		{
			CNode *Node = AKey[Axis] < Key[Axis] ? Left : Right;
			if (IsLeaf() || Key == AKey || Node == NULL)
				return this;
			Node->FindInsetionPoint(AKey);
		}

		bool IsLeaf()
		{
			return Left == NULL && Right == NULL;
		}
	};

public:
	IMPLEMENT_CONTAINER_ASSOC_TYPEDEFS

	class iterator
	{
		friend CKDTree;

	private:
		CKDTree *KDTree;
		vector<CNode *> Path;

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

		void FindLeftMostNode(CNode *Node)
		{
			if (Node == NULL)
				return;
			Path.push_back(Node);
			FindLeftMostNode(Node->Left);
		}

		void FindRightMostNode(CNode *Node)
		{
			if (Node == NULL)
				return;
			Path.push_back(Node);
			FindRightMostNode(Node->Right);
		}

		void RecursiveMoveTo(CNode *Node, const KeyType &Key)
		{
			if (Node == NULL)
				return;
			Path.push_back(Node);
			if (Node->IsLeaf() || Node->Key == Key)
				return;
			Node = Key[Node->Axis] < Node->Key[Node->Axis] ? Node->Left : Node->Right;
			RecursiveMoveTo(Node, Key);
		}

		void MoveTo(const KeyType &Key)
		{
			if (KDTree == NULL)
				return;
			RecursiveMoveTo(KDTree->Start, Key);
			if (Path.size() == 0 || Path.back()->Key != Key || Path.back()->Deleted)
				Path.clear();
		}

		int GetParentNode(CNode *&Node)	// -1 if node is a left child of it's parent. 1 otherwise. But 0 if have no parent
		{
			if (Path.size() < 2)
			{
				Node = NULL;
				return 0;
			}
			Node = Path[Path.size() - 2];
			if (Node->Left == Path.Rear())
				return -1;
			else
				return 1;
		}

		CNode *GetNode()
		{
			if (Path.size() == 0)
				return NULL;
			return Path.back();
		}

		iterator &OmmitWeakDeletedOnIncrement()
		{
			if (Path.size() > 0 && Path.back()->Deleted)
				return ++*this;
			return *this;
		}

		iterator &OmmitWeakDeletedOnDecrement()
		{
			if (Path.size() > 0 && Path.back()->Deleted)
				return --*this;
			return *this;
		}

	public:
		iterator(CKDTree *AKDTree = NULL) : KDTree(AKDTree)
		{
		}

		bool Ok() const
		{
			return Path.size() != 0 && !Path.back()->Deleted;
		}

		value_type operator *()
		{
			if (!Ok())
				throw CExceptionInvalidIteratorOperation();
			return std::make_pair(Path.back()->Key,  Path.back()->Data);
		}

		iterator& operator --()
		{
			if (Path.size() == 0)
			{
				FindRightMostNode(KDTree->Start);
				return OmmitWeakDeletedOnDecrement();
			}
			CNode *Node = Path.back();
			if (Node->Left != NULL)
			{
				FindRightMostNode(Node->Left);
				return OmmitWeakDeletedOnDecrement();
			}
			do 
				Node = Path.back(), Path.pop_back();
			while (Path.size() > 0 && Path.back()->Right != Node);

			return OmmitWeakDeletedOnDecrement();
		}

		iterator& operator ++()
		{
			if (Path.size() == 0)
			{
				FindLeftMostNode(KDTree->Start);
				return OmmitWeakDeletedOnIncrement();
			}
			CNode *Node = Path.back();
			if (Node->Right != NULL)
			{
				FindLeftMostNode(Node->Right);
				return OmmitWeakDeletedOnIncrement();
			}
			do 
				Node = Path.back(), Path.pop_back();
			while (Path.size() > 0 && Path.back()->Left != Node);

			return OmmitWeakDeletedOnIncrement();
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
		bool operator ==(const iterator &rhs) const
		{
			return !Ok() && !rhs.Ok() || rhs.Path.back() == Path.back();
		}
	};


	CKDTree(unsigned int ADimensionsNumber = 2) : DimensionsNumber(ADimensionsNumber), 
		ElementsNumber(0), Start(NULL)
	{}

	CKDTree(vector<ValueTypeNonConst> &Values, unsigned int ADimensionsNumber = 2) : 
		DimensionsNumber(ADimensionsNumber), ElementsNumber(0), Start(NULL), WeakDeletedNumber(0)
	{
		BuildTreeFrom(Values);
	}

	~CKDTree()
	{
		clear();
	}

	bool empty()
	{
		return ElementsNumber == 0;
	}

	void insert(const KeyType &Key, const DataType &Data)
	{
		(*this)[Key] = Data;
	}

	unsigned int erase(const KeyType &Key)
	{
		iterator i = find(Key);
		if (!i.Ok())
			return 0;
		erase(i);
		return 1;
	}

	void erase(iterator &i)
	{
		if (!i.Ok())
			return;
		ElementsNumber--;
		WeakDeletedNumber++;
		CNode *Node = i.GetNode();
		Node->Deleted = true;
		if (ElementsNumber != WeakDeletedNumber)
			return;
		RebuildTree();
		return;
	}

	unsigned int size()
	{
		return ElementsNumber;
	}

	void BuildTreeFrom(vector<ValueTypeNonConst> &Values)
	{
		clear();
		Start = RecursiveBuild_(Values, 0, Values.size() - 1, 0);
		ElementsNumber = Values.size();
	}

	void RebuildTree()
	{
		vector<ValueTypeNonConst> Values;
		Values.reserve(ElementsNumber);
		Start->CollectPoints(Values);
		BuildTreeFrom(Values);
	}

	void clear()
	{
		if (ElementsNumber == 0)
			return;
		ElementsNumber = 0;
		WeakDeletedNumber = 0;
		Start->Clear();
		delete Start;
	}

	void NNSearch(vector<ValueTypeNonConst> &Result, unsigned int k)
	{
		// T_T
	}

	DataType& operator [](const KeyType &Key)
	{
		CNode *Node = Start->FindInsetionPoint(Key);
		if (Node == NULL)
			throw(CExceptionInvalidOperation());
		if (Key != Node->Key)
		{
			unsigned int Axis = (Node->Axis + 1) % DimensionsNumber;
			CNode *NewNode = new CNode(Key, DataType(), NULL, NULL, Axis);
			if (NewNode->Key[Axis] < Node->Key[Axis])
				Node->Left = NewNode;
			else
				Node->Right = NewNode;
			Node = NewNode;
			ElementsNumber++;
		}
		Node->Deleted = false;
		return Node->Data;
	}

	const DataType& operator [](const KeyType &Key) const
	{
		return (*this)operator [](Key);
	}

	iterator find(const KeyType &Key)
	{
		iterator i(this);
		i.MoveTo(Key);
		return i;
	}

	iterator begin()
	{
		return ++iterator(this);
	}

	iterator end()
	{
		return --iterator(this);
	}

	typedef KeyType KeyType;

private:
	unsigned int DimensionsNumber;
	unsigned int ElementsNumber;
	unsigned int WeakDeletedNumber;
	CNode *Start;

	void SortPoints_(vector<ValueTypeNonConst> &Values, int LeftIndex, int RightIndex, unsigned int AxisIndex)
	{
		if (LeftIndex >= RightIndex)
			return;
		vector<value_type>::value_type::first_type::value_type Measure =
			(Values[(RightIndex + LeftIndex) / 2].first)[AxisIndex];
		int i = LeftIndex;
		int j = RightIndex;
		do
		{
			while ((Values[i].first)[AxisIndex] < Measure)
				i++;
			while ((Values[j].first)[AxisIndex] > Measure)
				j--;

			if (i<=j)
			{
				value_type t = Values[i];
				Values[i] = Values[j];
				Values[j] = t;
				i++;
				j--;
			}
		} while(i < j);
		if (i < RightIndex)
			SortPoints_(Values, i, RightIndex, AxisIndex);
		if (LeftIndex < j)
			SortPoints_(Values, LeftIndex, j, AxisIndex);
	}

	CNode* RecursiveBuild_(vector<ValueTypeNonConst> &Values, int LeftIndex, int RightIndex, int DepthIndex)
	{
		if (RightIndex < LeftIndex)
			return NULL;
		unsigned int Axis = DepthIndex % DimensionsNumber;
		SortPoints_(Values, LeftIndex, RightIndex, Axis);
		unsigned int Median = LeftIndex + (RightIndex - LeftIndex + 1) / 2;
		CNode *NewNode = new CNode(Values[Median].first, Values[Median].second, NULL, NULL, Axis);
		NewNode->Left = RecursiveBuild_(Values, LeftIndex, Median - 1, DepthIndex + 1);
		NewNode->Right = RecursiveBuild_(Values, Median + 1, RightIndex, DepthIndex + 1);
		return NewNode;
	}
};

#endif	// _DTL_KDTREE_H_
