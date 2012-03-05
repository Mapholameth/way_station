#pragma once

#include "RTTI.h"

class CFoo : public CNullClass
{
	DECLARE_RTTI(CFoo, CNullClass)

public:
	CFoo() : test(2), testString("ololo"), testFloat(3.1415f)
	{
		testIntVector.resize(4);
	}

	DECLARE_TRIVIAL_PROPERTY(int, test)
	DECLARE_TRIVIAL_PROPERTY(string, testString)
	DECLARE_TRIVIAL_PROPERTY(float, testFloat)
	DECLARE_TRIVIAL_PROPERTY(vector<int>, testIntVector)

	static void RegisterReflection()
	{
		REGISTER_PROPERTY(int, test, CFoo)
		REGISTER_PROPERTY(string, testString, CFoo)
		REGISTER_PROPERTY(float, testFloat, CFoo)
		REGISTER_PROPERTY(vector<int>, testIntVector, CFoo)
	}
};

template<>
void CStreamSerializer::Serialize<CFoo>(ostream &out, const string& name, const CFoo& value);

template<>
void CStreamSerializer::Deserialize<CFoo>(istream &out, const string& name, CFoo& value );

class CBar : public CFoo
{
	DECLARE_RTTI(CBar, CFoo)
public:
	DECLARE_TRIVIAL_PROPERTY(unsigned, TheOnlyOne)
	DECLARE_TRIVIAL_PROPERTY(CFoo, fooMember)

	static void RegisterReflection()
	{
		REGISTER_PROPERTY(unsigned, TheOnlyOne, CBar)
		REGISTER_PROPERTY(CFoo, fooMember, CBar)
	}
};

class CFooBar : public CBar
{
	DECLARE_RTTI(CFooBar, CBar)
	static void RegisterReflection()
	{

	}
};

class CDeckard : public CFoo
{
	DECLARE_RTTI(CDeckard, CFoo)
	static void RegisterReflection()
	{

	}
};

class CFooBarDeckard : public CFooBar
{
	DECLARE_RTTI(CFooBarDeckard, CFooBar)

	static void RegisterReflection()
	{

	}
};

class CTest : public CNullClass
{
	DECLARE_RTTI(CTest, CNullClass)
};

class CTestAbstract : public CTest
{
	DECLARE_RTTI(CTestAbstract, CTest)
public:
	virtual void MakingAbstract() = 0;
};

class CTestHeir : public CTestAbstract
{
	DECLARE_RTTI(CTestHeir, CTestAbstract)
public:
	virtual void MakingAbstract(){}
};
