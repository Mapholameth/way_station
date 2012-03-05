#include "property.h"

const string& CProperty::GetName() const
{
	return name;
}
void CProperty::SetName(const string& aName)
{
	name = aName;
}