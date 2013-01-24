#ifndef _TREE_MAP_H_
#define _TREE_MAP_H_

#ifdef _DEBUG
	#include <assert.h>
#endif	//	_DEBUG

template<typename KeyType, typename DataType>	// i.e. Data type & Key type
class CTreeMap
{
private:
	class CNode
	{
	public:
		CNode *Parent;
		CNode *Left;
		CNode *Right;
		DataType Data;
		KeyType Key;
		int Balance;
	};
	size_t ElementsNumber;
	CNode *First;
	CNode *Last;
	CNode *Root;
	
	/* Обновляет потомка предка узла, при его замене в случае, если мы не знаем, левым был потомок или правым*/
	void UpdateParent(CNode *descendant, CNode*newDescendant)
	{
		if (descendant->Parent != NULL)
		{
			if (descendant->Parent->Left == descendant)
				descendant->Parent->Left = newDescendant;
			if (descendant->Parent->Right == descendant)
				descendant->Parent->Right = newDescendant;
		}
		else
		{
			Root = newDescendant;		
		}
		newDescendant->Parent = descendant->Parent;
		/* Код который вызывал эту процедуру должен обновить предка старого
			потомка т.к. в разных случаях поворота он может быть разным, а тут мы его обнулим*/
		descendant->Parent = NULL;
	}

	void RightRotation(CNode *root)
	{
		CNode *pivot = root->Left;
		UpdateParent(root, pivot);
		root->Left = pivot->Right;
		if (root->Left != NULL)
			root->Left->Parent = root;
		pivot->Right = root;
		root->Parent = pivot;
		pivot->Balance = 0;
		root->Balance = 0;
	}
	void LeftRotation(CNode *root)
	{
		CNode *pivot = root->Right;
		UpdateParent(root, pivot);
		root->Right = pivot->Left;
		if (root->Right != NULL)
			root->Right->Parent = root;
		pivot->Left = root;
		root->Parent = pivot;
		pivot->Balance = 0;
		root->Balance = 0;
	}

	void LeftRightRotation(CNode *root)
	{
		CNode *pivot = root->Left, *bottom = pivot->Right;
		int OldBottomBalance = bottom->Balance;
		LeftRotation(root->Left);
		RightRotation(root);
		bottom->Balance = 0;
		switch (OldBottomBalance)
		{
		case 0:
			root->Balance = 0;
			pivot->Balance = 0;
			break;
		case 1:
			root->Balance = 0;
			pivot->Balance = -1;
			break;
		case -1:
			root->Balance = 1;
			pivot->Balance = 0;
			break;
		}
	}

	void RightLeftRotation(CNode *root)
	{
		CNode *pivot = root->Right, *bottom = pivot->Left;
		int OldBottomBalance = bottom->Balance;
		RightRotation(root->Right);
		LeftRotation(root);
		bottom->Balance = 0;
		switch (OldBottomBalance)
		{
		case 0:
			root->Balance = 0;
			pivot->Balance = 0;
			break;
		case 1:
			root->Balance = -1;
			pivot->Balance = 0;
			break;
		case -1:
			root->Balance = 0;
			pivot->Balance = 1;
			break;
		}
	}

	void SwapNodes(CNode *A, CNode *B)
	{
		DataType Temp = A->Data;
		KeyType TempKey = A->Key;
		A->Data = B->Data;
		B->Data = Temp;
		A->Key = B->Key;
		B->Key = TempKey;
	}

	void DestroyNext(CNode *NextNode)
	{
		if (NextNode == NULL)
			return;
		if (NextNode->Left == NULL && NextNode->Right == NULL)
		{
			delete NextNode;
			return;
		}
		if (NextNode->Left != NULL)
			DestroyNext(NextNode->Left);
		if (NextNode->Right != NULL)
			DestroyNext(NextNode->Right);
		delete NextNode;
	}

public:
	IMPLEMENT_CONTAINER_ASSOC_TYPEDEFS
	class iterator
	{
		friend CTreeMap;
	private:
		CTreeMap *Map;
		CNode *Node;
		enum
		{
			BEFORE_FIRST,
			PAST_REAR,
			NORMAL,
		}State;
	public:
		iterator(CTreeMap *ATreeMap) : Map(ATreeMap), Node(Map->First), State(Node == NULL ? PAST_REAR : NORMAL) {}

		bool Ok()
		{
			return State == NORMAL;
		}

		value_type operator *()
		{
			if (!Ok())
				throw "smthng bad";
			return std::make_pair(Node->Key, Node->Data);
		}

		const KeyType& GetKey()
		{
			return Node->Key;
		}

		iterator& operator --()
		{
			if (State == PAST_REAR)
			{
				if (Map->ElementsNumber == 0)
				{
					State = BEFORE_FIRST;
					return *this;
				}
				State = NORMAL;
				Node = Map->Last;
				return *this;
			}
			if (State == BEFORE_FIRST)
				return *this;
			if (Node == Map->First)
			{
				State = BEFORE_FIRST;
				return *this;
			}
			if (Node->Left != NULL)
			{
				Node = Node->Left;
				while (Node->Right != NULL)
					Node = Node->Right;
			}
			else
			{		
				while (Node->Parent->Right != Node)
				{
					Node = Node->Parent;
				}
				Node = Node->Parent;
			}
			return *this;
		}

		iterator& operator ++()
		{
			if (State == BEFORE_FIRST)
			{
				if (Map->ElementsNumber == 0)
				{
					State = PAST_REAR;
					return *this;
				}
				State = NORMAL;
				Node = Map->First;
				return *this;
			}
			if (State == PAST_REAR)
				return *this;
			if (Node == Map->Last)
			{
				State = PAST_REAR;
				return *this;
			}
			if (Node->Right != NULL)
			{
				Node = Node->Right;
				while (Node->Left != NULL)
					Node = Node->Left;
			}
			else
			{		
				while (Node->Parent->Left != Node)
				{
					Node = Node->Parent;
				}
				Node = Node->Parent;
			}
			return *this;
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


		void ShiftPosition(int shift)
		{
			int absshift = (shift > 0) ? shift : -shift;
			while (absshift != 0)
			{
				(shift > 0) ? (*this)++ : (*this)--;
				absshift--;
			}
		}
		
		void SetTo(int pos)
		{
			State = BEFORE_FIRST;
			Node = NULL;
			ShiftPosition(pos+1);
		}

	};

	CTreeMap() : ElementsNumber(0), First(NULL), Last(NULL), Root(NULL){}

	iterator GetElement(const KeyType &Key)
	{
		iterator iterator(this);
		iterator.Node = Root;
		iterator.State = iterator::NORMAL;	
		while (iterator.Node != NULL)
		{
			if (iterator.Node->Key == Key)
				return iterator;
			if (Key < iterator.Node->Key)
				iterator.Node = iterator.Node->Left;
			else
				iterator.Node = iterator.Node->Right;
		}
		iterator.State = iterator::PAST_REAR;
		return iterator;
	}


	void Add(const KeyType &Key, const DataType &NewElement)
	{
		CNode *NewNode = NULL, *TempNode = NULL, *ParentNode = NULL, *PrevNode = NULL;
		TempNode = Root;
		while (TempNode != NULL)
		{
			if (TempNode->Key == Key)
			{
				TempNode->Data = NewElement;
				return;
			}
			ParentNode = TempNode;
			if (Key < TempNode->Key)
				TempNode = TempNode->Left;
			else
				TempNode = TempNode->Right;
		}
		NewNode = new CNode;
		if (NewNode == NULL)
			return;	
		NewNode->Key = Key;
		NewNode->Data = NewElement;
		NewNode->Balance = 0;
		NewNode->Left = NULL;
		NewNode->Right = NULL;
		NewNode->Parent = ParentNode;
		if (ElementsNumber == 0)
		{
			First = Last = Root = NewNode;
			ElementsNumber++;
			return;
		}
		if (Key < ParentNode->Key)
		{
			ParentNode->Left = NewNode;
			if (First == ParentNode)
				First = NewNode;
		}
		else
		{
			if (Last == ParentNode)
				Last = NewNode;
			ParentNode->Right = NewNode;
		}
		PrevNode = NewNode;
		TempNode = NewNode->Parent;
		while (TempNode != NULL)
		{
			if (TempNode->Left == PrevNode)
				TempNode->Balance--;
			if (TempNode->Right == PrevNode)
				TempNode->Balance++;

			if (TempNode->Balance == -2)
			{
				if (TempNode->Left != NULL && TempNode->Left->Balance == 1)
				{
					LeftRightRotation(TempNode);
					if (TempNode->Balance != 0)
						TempNode->Parent->Balance--;
				}
				else
					RightRotation(TempNode);

			}
			if (TempNode->Balance == 2)
			{			
				if (TempNode->Right != NULL && TempNode->Right->Balance == -1)				
				{
					RightLeftRotation(TempNode);
					if (TempNode->Balance != 0)
						TempNode->Parent->Balance++;
				}
				else
					LeftRotation(TempNode);

			}
			if (TempNode->Balance == 0)
				break;
			PrevNode = TempNode;
			TempNode = TempNode->Parent;
		}
		ElementsNumber++;
	}

	void Delete(const KeyType &Key)
	{
		CNode *TempNode = NULL, *PrevNode = NULL, *DeletedNode = NULL;
		iterator iterator(this);
		iterator = GetElement(Key);
		if (iterator.State != iterator::NORMAL)
			return;

		while (iterator.Node->Left != NULL || iterator.Node->Right != NULL)
		{
			TempNode = iterator.Node;
			if (iterator.Node->Balance == 1)
				iterator++;
			else
				iterator--;
			SwapNodes(TempNode, iterator.Node);
		}
		if (iterator.Node->Left == NULL && iterator.Node->Right == NULL)
		{		
			if (iterator.Node == First)
			{
				iterator++;
				TempNode = iterator.Node;
				iterator--;
				First = TempNode;
			}
			if (iterator.Node == Last)
			{
				iterator--;
				TempNode = iterator.Node;
				iterator++;
				Last = TempNode;
			}
			PrevNode = DeletedNode = iterator.Node;
			TempNode = iterator.Node->Parent;
			//free(iterator.Node);
		}
		while (TempNode != NULL)
		{

			if (TempNode->Left == PrevNode)
				TempNode->Balance++;
			if (TempNode->Left == DeletedNode)
				TempNode->Left = NULL;
			if (TempNode->Right == PrevNode)
				TempNode->Balance--;
			if (TempNode->Right == DeletedNode)
				TempNode->Right = NULL;


			if (abs(TempNode->Balance) == 1)
				break;

			if (TempNode->Balance == -2)
				if (TempNode->Left != NULL && TempNode->Left->Balance == 1)
				{
					LeftRightRotation(TempNode);
					TempNode->Parent->Balance++;
				}
				else
				{
					int OldPivotBalance = TempNode->Left->Balance;
					RightRotation(TempNode);
					if (abs(OldPivotBalance) != 1)
					{
						TempNode->Balance = -1;
						TempNode->Parent->Balance = 1;
					}				
					TempNode->Parent->Balance++;
				}

				if (TempNode->Balance == 2)
					if (TempNode->Right != NULL && TempNode->Right->Balance == -1)
					{
						RightLeftRotation(TempNode);
						TempNode->Parent->Balance--;
					}
					else
					{
						int OldPivotBalance = TempNode->Right->Balance;
						LeftRotation(TempNode);
						if (abs(OldPivotBalance) != 1)
						{
							TempNode->Balance = 1;
							TempNode->Parent->Balance = -1;
						}
						TempNode->Parent->Balance--;
					}
					PrevNode = TempNode;
					TempNode = TempNode->Parent;		
		}
		ElementsNumber--;
		if (ElementsNumber == 0)
			First = Last = Root = NULL;
	}

	void clear()
	{
		DestroyNext(Root);
	}

	~CTreeMap()
	{
		clear();
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
		iterator iterator(this);
		iterator.Node = Last;
		iterator.State = iterator->Node == NULL ? iterator::PAST_REAR : iterator::NORMAL;
		return iterator;
	}

	void DeleteFront()
	{
		Delete(First->Key);
	}

	void DeleteRear()
	{
		Delete(Last->Key);
	}

	DataType operator [](const KeyType &Key)	// was DataType& operator [](const KeyType &Key)
	{
		iterator iterator = GetElement(Key);
		if (!iterator.Ok())
			Add(Key, NULL);
		return (*GetElement(Key)).second;
	}

#ifdef _DEBUG
	void Check()
	{
		CheckBalance(Root);
	}

	static __inline int MAX(int a, int b)
	{
		if (a > b)
			return a;
		else
			return b;
	}

	int CheckBalance(CNode *NextNode)
	{
		if (NextNode == NULL)
			return 0;
		if (NextNode->Left == NULL && NextNode->Right == NULL)	
		{
			assert(NextNode->Balance == 0);
			return 1;
		}
		{
			int leftsubheight = CheckBalance(NextNode->Left);
			int rightsubheight = CheckBalance(NextNode->Right);
			assert(rightsubheight - leftsubheight == NextNode->Balance);
			return MAX(leftsubheight, rightsubheight) + 1;
		}	
	}
#endif // _DEBUG
};

#endif // _TREE_MAP_H_