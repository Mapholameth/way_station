#ifndef _TEST_H_
#define _TEST_H_
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <string>
#include <time.h>
#include <typeinfo>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
using std::string;
using std::exception;
using std::cout;
using std::endl;
using std::stringstream;
using std::ostream;
#include "dtl_Allocator.h"

void Check(bool Ok, const string &ContainerInfo, int Line, const string &FunctionName);
void PrintTotals();
#define CHECK(OK) Check(OK, typeid(C).name(), __LINE__, __FUNCTION__)

template <typename DataType>
struct IsContainer
{
	static const bool associative = false;
};

#define IMPLEMENT_CONTAINER_IS_ASSOCIATIVE(TYPE) \
	template <typename KeyType, typename DataType> \
	struct IsContainer<TYPE<KeyType, DataType> > \
	{ \
		static const bool associative = true; \
	};

#include "dtl_SkipList.h"
#include "dtl_TreeMap.h"
#include "dtl_HashMap.h"
#include "dtl_Trie.h"
#include "dtl_bad_Trie.h"
#include "dtl_KDTree.h"

IMPLEMENT_CONTAINER_IS_ASSOCIATIVE(CSkipList)
IMPLEMENT_CONTAINER_IS_ASSOCIATIVE(CTreeMap)
IMPLEMENT_CONTAINER_IS_ASSOCIATIVE(dtl::hash_map)
IMPLEMENT_CONTAINER_IS_ASSOCIATIVE(CTrieBad)
IMPLEMENT_CONTAINER_IS_ASSOCIATIVE(CTrie)
IMPLEMENT_CONTAINER_IS_ASSOCIATIVE(CKDTree)

template <typename DataType>
string to_string(const DataType &Value)
{
	stringstream ss;
	ss << Value;
	return ss.str();
};

// Некий пользовательский класс для тестов.
class CTestClass
{
public:
	void *SomeData;
	int IntegralMember;
	string StringMember;

	CTestClass();
	CTestClass(const CTestClass &TestClass);
	~CTestClass();
	bool operator <(const CTestClass &Operand) const;
	bool operator ==(const CTestClass &Operand) const;
	bool operator !=(const CTestClass &Operand) const;
	CTestClass& operator =(const CTestClass &Operand);
	CTestClass& operator +=(const CTestClass &Operand);
	friend ostream& operator <<(ostream &os, const CTestClass &Source);
};

// Проверка проверки на ассоциативность.
template <typename C>
void TestForAssociativity()
{
	cout << typeid(C).name();
	if (IsContainer<C>::associative)
		cout << " ----- is associative.";
	else
		cout << " ----- is not associative.";
	cout << endl;
}

// Проверка проверки на конкретность.
template <typename DataType>
void TestForConcreteness()
{
	cout << typeid(DataType).name();
	if (Is<DataType>::concrete)
		cout << " is concrete.";
	else
		cout << " is NOT concrete.";
	cout << endl;
}

//////////////////////////////////////////////////////////////////////////
// Вспомогательные функции для тестов

template <typename C>
void ContainerOut(C &Container)
{
	for(C::iterator i = Container.begin(); i.Ok(); i++)
		cout << *i << endl;
}

template <typename C>
void FillContainerSeq(C &Container, const typename C::value_type &base)
{
	int i(10);
	C::value_type tmp = base;
	while(i--)
		Container.push_back(tmp += base);
}

template <typename C>
void FillContainerAssoc(C &Container, const typename C::value_type &base, const typename C::KeyType &key)
{
	int i(10);
	C::value_type tmp = base;
	C::value_type tmpkey = key;
	while(i--)
		Container.Add(tmp += base, tmpkey += key);
}


template<typename C>
bool CheckContainerEquality(C &First, C &Second)
{
	if (First.size() != Second.size())
		return false;
	for(unsigned int i = 0; i < First.size(); i++)
		if (First[i] != Second[i])
			return false;
	return true;
}

template<typename C>
bool CheckContainerEqualityAssoc(C &First, C &Second)
{
	if (First.size() != Second.size())
		return false;
	for(C::iterator i = First.begin(), j = Second.begin(); i.Ok() && j.Ok(); ++i, j++)
	{
		if ((*i) != (*j))
			return false;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
// Непостредственно основные тесты.

template <typename C>
void TestConstructor()
{
	C *Container = NULL;
	try
	{
		Container = new C();
		CHECK(Container->empty());
	}
	catch(...)
	{
		CHECK(false);
		return;
	}
	
	try
	{
		C::iterator i = Container->begin();
		CHECK(i == Container->end());
	}
	catch(...)
	{
			CHECK(false);
	}

	try
	{
		delete Container;
		CHECK(true);
	}
	catch(...)
	{
		CHECK(false);
	}
}

template <typename C>
void TestCopyConstructorSeq(const typename C::value_type &a, const typename C::value_type &b)
{
	C list;
	FillContainerSeq(list, a);
	C list1;
	FillContainerSeq(list1, b);
	list1 = list1;
	list1 = C(list1);
	list1 = list;
	CHECK(CheckContainerEquality(list1, list));
	C list2(list1);
	CHECK(CheckContainerEquality(list1, list2));
	list2.clear();
	list1 = list2;
	CHECK(list1.empty());
}

template <typename C>
void TestCopyConstructorAssoc(	const typename C::value_type &a, const typename C::value_type &b,
								const typename C::KeyType &ak, const typename C::KeyType &bk)
{
	C list;
	FillContainerAssoc(list, a, ak);
	C list1;
	FillContainerAssoc(list1, b, bk);
	list1 = list1;
	list1 = C(list1);
	list1 = list;
	CHECK(CheckContainerEqualityAssoc(list1, list));
	C list2(list1);
	CHECK(CheckContainerEqualityAssoc(list1, list2));
	list2.clear();
	list1 = list2;
	CHECK(list1.empty());
}


template <typename C>
void TestInsertionAndDeletion(	const typename C::KeyType &k1, const typename C::KeyType &k2,
								const typename C::KeyType &k3, const typename C::KeyType &k4,
								const typename C::KeyType &k5)	// I think 5 is enough for testing
{
	try
	{
		C Container;
		Container.Add(k1, k1);
		Container.Add(k1, k1);
		if (Container.size() != 1)
			throw std::bad_exception();
		Container.Add(k2, k2);
		Container.Add(k3, k3);
		Container.Add(k4, k4);
		Container.Add(k5, k5);
		if (Container.size() != 5)
			throw std::bad_exception();
//		Container.DebugPrint();
		C::KeyType j(0), tmp(Container[k1]);
		for(C::iterator i = Container.begin(); i.Ok(); i++, j++)
		{			
			if (j != 0 && tmp >= *i)
				throw std::bad_exception();
			tmp = *i;
		}
	}
	catch(const exception &e)
	{
		CHECK(false);
	}
}

// template <typename C>
// void Test

template <typename C>
void TestInsert()
{
	C Container;
	typename C::value_type Value;
	C::iterator i = Container.end();
/*	Container.insert(i, Value);*/
}

//////////////////////////////////////////////////////////////////////////
// Тесты производетельности.

template <typename C>
void PerformanceTestSequence()
{
	#define PERF_SCALE 1000000
	#define DELIMITER		"================================================================================" << endl
	#define DELIMITER_SMALL "--------------------------------------------------------------------------------" << endl
	cout << DELIMITER_SMALL;
	cout << typeid(C).name() << ":" << endl;
	C *Container = new C();
	C::value_type Entry;
	clock_t time = clock();
	for(unsigned int i = 0; i < PERF_SCALE; i++)
	{
		Container->push_back(Entry);		
	}
	time = clock() - time;
	cout << "\t" << PERF_SCALE << " Calls of push_back() taken " << time << " ms." << endl;

	time = clock();
	unsigned int Counter = 0;
	for(C::iterator i = Container->begin(); i != Container->end(); ++i)
		Counter++;
	time = clock() - time;
	CHECK(Counter == PERF_SCALE);
	cout << "\t" << "Iterating through " << PERF_SCALE << " elements taken " << time << " ms." << endl;

// 	time = clock();
// 	Counter = 0;
// 	for(unsigned int i = 0; i < Container->Size(); i++)
// 	{
// 		/*Entry*/(*Container)[i];
// 		Counter++;
// 	}
// 	time = clock() - time;
// 	CHECK(Counter == PERF_SCALE);
// 	cout << "\t" << "Iterating through " << PERF_SCALE << " elements using [], taken " << time << " ms." << endl;

	time = clock();
	for(unsigned int i = 0; i < PERF_SCALE / 2; i++)
	{
		Container->pop_back();		
	}
	time = clock() - time;
	cout << "\t" << PERF_SCALE / 2<< " Calls of DeleteRear() taken " << time << " ms." << endl;

	time = clock();
	delete Container;
	time = clock() - time;
	cout << "\tDestructor call taken " << time << " ms." << endl;
};

void GetFileContentsParsedToWords(vector<string> &Array, const char *Filename);

template <typename C>	// C is associative container
void PerformanceTest(const vector<string> &InputWords)
{
	C TestContainer;
	cout << typeid(C).name() << ":" << endl;
	clock_t time = clock();	
	for(int i = 0; i < InputWords.size(); i++)
	{
		if (TestContainer[InputWords[i]] == 0)
			TestContainer[InputWords[i]] = i + 1;		
	}	
	time = clock() - time;
	cout << "\tInsertion of " << InputWords.size() << " words" << " taken " << time << " ms." << endl;

	time = clock();	
	for(int i = 0; i < InputWords.size(); i++)
	{
		if ((TestContainer[InputWords[i]] != i + 1) && TestContainer[InputWords[i]] != 0)	
			CHECK(false);
 		else
 			TestContainer[InputWords[i]] = 0;
	}	
	time = clock() - time;
	cout << "\tRetrieval of " << InputWords.size() << " words" << " taken " << time << " ms." << endl;

// 	time = clock();	
// 	for(C::iterator i = TestContainer.begin(); i.Ok(); ++i)
// 	{
// 		cout << (*i).first << " --- " << (*i).second <<endl;
// 	}	
// 	time = clock() - time;
// 	cout << "\tIterating through " << InputWords.size() << " words" << " taken " << time << " ms." << endl;
}

template <typename C>
void KDTreePerfTest()
{
#define ITERATIONS_COUNT 8
	unsigned int dn = 1000;
	unsigned int i = 4;
	unsigned int n = dn * i;
	vector<C::ValueTypeNonConst> Points;
	C *Container = NULL;
	vector<clock_t>TimingsBuilding;
	vector<clock_t>TimingsLookup;
	vector<clock_t>TimingsInsert;
	vector<clock_t>TimingsErase;
	while (i < ITERATIONS_COUNT)
	{
		for(unsigned int j = 0; j < n; j++)
			Points[j] = std::make_pair(Vector2(j, j), j);

		clock_t time = clock();
			Container = new C(Points);
		time = clock() - time;
		TimingsBuilding[i] = time;

		time = clock();
		for(unsigned int j = n; j < n *2 ; j++)
			((*Container)[Vector2(j, j)]) = j;
		time = clock() - time;
		TimingsInsert[i] = time;

		time = clock();
		for(unsigned int j = 0; j < n; j++)
		{
 			C::iterator it = Container->find(Vector2(j, j));
 			if (!it.Ok())
 				CHECK(false);
 			else if (it.Ok() && (*it).second != j)
 				CHECK(false);
		}
		time = clock() - time;
		TimingsLookup[i] = time;
		i++;
		time = clock();
		for(unsigned int j = 0; j < n; j++)
		{
			C::iterator it = Container->find(Vector2(j, j));
			Container->erase(it);
		}
		time = clock() - time;
		TimingsErase[i] = time;
		CHECK(Container->size() == n);
		time = clock();
			delete Container;
		time = clock() - time;
		cout << endl;

		n += dn;
	}
	i = 4;
	while(i < ITERATIONS_COUNT - 1)
	{
		if ((TimingsBuilding[i + 1] / (TimingsBuilding[i] + 31.0f)) > 2.0f)
			CHECK(false);
		if ((TimingsErase[i + 1] / (TimingsErase[i] + 31.0f)) > 2.0f)
			CHECK(false);
		if ((TimingsLookup[i + 1] / (TimingsLookup[i] + 31.0f)) > 2.0f)
			CHECK(false);
		if ((TimingsInsert[i + 1] / (TimingsInsert[i] + 31.0f)) > 2.0f)
			CHECK(false);
		i++;
	}
}

#endif	// _TEST_H_

