#include "Test.h"

DEFINE_RTTI(CFoo, CNullClass)
DEFINE_RTTI(CBar, CFoo)
DEFINE_RTTI(CFooBar, CBar)
DEFINE_RTTI(CDeckard, CFoo)
DEFINE_RTTI(CFooBarDeckard, CFooBar)
DEFINE_RTTI(CTest, CNullClass)
DEFINE_RTTI(CTestAbstract, CTest)
DEFINE_RTTI(CTestHeir, CTestAbstract)


template<>
void CStreamSerializer::Serialize<CFoo>(ostream &out, const string& name, const CFoo& value)
{
	;
}

template<>
void CStreamSerializer::Deserialize<CFoo>(istream &out, const string& name, CFoo& value )
{
	;
}
