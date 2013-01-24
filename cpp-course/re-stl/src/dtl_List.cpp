#include "dtl_List.h"

void CListBase::CIteratorBase::_NextNode()
{
	Node = Node->Next;
}

void CListBase::CIteratorBase::_PrevNode()
{
	Node = Node->Prev;
}

void CListBase::clear()
{
	while (!empty())
		DeleteRear();
}

void CListBase::IncSize()
{
	++ElementsNumber;
}

void CListBase::DecSize()
{
	--ElementsNumber;
}

bool CListBase::empty() const
{
	return Start.Prev == &Start;	// ElementsNumber == 0
}

size_t CListBase::size() const
{
	return ElementsNumber;
}

bool CListBase::CIteratorBase::Ok() const
{
	return Node != &List->Start;
}
