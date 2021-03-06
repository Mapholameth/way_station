#include <fstream>
#include <Windows.h>
#include <map>
#include <vector>
#include "dtl_Test.h"
#include "dtl_String.h"
#include "dtl_List.h"
#include "dtl_Array.h"
#include "dtl_HashMap.h"
#include "dtl_TreeMap.h"
#include "dtl_SkipList.h"
#include "dtl_Trie.h"
#include "dtl_Test.h"
#include "dtl_bad_Trie.h"
#include "dtl_KDTree.h"

int main()
{
	//freopen("out.txt", "w", stdout);

	try
	{
//======// Test Is<T>::concrete for various types
		cout << DELIMITER;
		cout << "Type concreteness tests output:" << endl;
		cout << DELIMITER;
		TestForConcreteness<int>();
		TestForConcreteness<unsigned int>();
		TestForConcreteness<char>();
		TestForConcreteness<unsigned char>();
		TestForConcreteness<short>();
		TestForConcreteness<unsigned short>();
		TestForConcreteness<long>();
		TestForConcreteness<unsigned long long>();
		TestForConcreteness<string>();
		TestForConcreteness<const string>();
		TestForConcreteness<void>();
		TestForConcreteness<void*>();
		TestForConcreteness<const void *>();
		TestForConcreteness<const void const *>();
		TestForConcreteness<string *>();
		TestForConcreteness<const string *>();
		TestForConcreteness<const string const *>();
		TestForConcreteness<const string * const>();
		TestForConcreteness<string &>();
		TestForConcreteness<const string &>();
		TestForConcreteness<CTestClass>();
//======// Test IsContainer<C>::associative for CList CArray CTreeMap CHashMap etc
		cout << DELIMITER;
		cout << "Type associativity tests output:" << endl;
		cout << DELIMITER;
		TestForAssociativity<CList<int> >();
/*		TestForAssociativity<CString>();*/
		TestForAssociativity<vector<int> >();
		TestForAssociativity<CTrieBad<int, int> >();
		TestForAssociativity<dtl::hash_map<int, int> >();
		TestForAssociativity<CSkipList<int, int> >();
		TestForAssociativity<CTreeMap<int, int> >();
		TestForAssociativity<CTrie<int, int> >();
		TestForAssociativity<CKDTree<Deku2D::Vector2, string> >();
		cout << DELIMITER;
//======// Test string module here
		// blah blah blah
//======// Test sequence containers (CArray, CList) here
// List
// 		TestConstructor<CList<int> >();
// 		TestConstructor<CList<string> >();
// 		TestConstructor<CList<CTestClass> >();
// 		TestCopyConstructorSeq<CList<int> >(1, 2);
// 		TestCopyConstructorSeq<CList<string> >("a", "b");
// 		TestCopyConstructorSeq<CList<CTestClass> >(CTestClass(), CTestClass());
// Dynamic Array
		TestConstructor<dtl::vector<int> >();
		TestConstructor<dtl::vector<string> >();
		TestConstructor<dtl::vector<CTestClass> >();
		TestCopyConstructorSeq<dtl::vector<int> >(1, 2);
		TestCopyConstructorSeq<dtl::vector<string> >("a", "b");
		TestCopyConstructorSeq<dtl::vector<CTestClass> >(CTestClass(), CTestClass());
		TestInsert<dtl::vector <int> >();
//======// Test assoc. containers (CHashMap, CTreeMap, CTrie, CSkipList) here
// Hash Map
// Tree Map
// Skip List
// 		TestConstructor<CSkipList<int, int> >();
//  		TestConstructor<CSkipList<int, string> >();
//  		TestConstructor<CSkipList<int, CTestClass> >();
//		TestCopyConstructorAssoc<CSkipList<int, int> >(1, 2, 1, 2);
// 		TestInsertionAndDeletion<CSkipList<int, int> >(1, 2, 3, 4, 5);
// 		TestInsertionAndDeletion<CSkipList<int, int> >(5, 4, 3, 2, 1);
// 		TestInsertionAndDeletion<CSkipList<int, int> >(1, 4, 2, 5, 3);
// Trie
// 		TestConstructor<CTrie<string, int> >();
// 		TestConstructor<CTrie<string, string> >();
// 		TestConstructor<CTrie<string, CTestClass> >();		
		//TestInsertionAndDeletion<CTrie<char *, char*> >("a", "ab", "abc", "bc", "");
// 		TestInsertionAndDeletion<CTrie<int, char*> >(5, 4, 3, 2, 1);
// 		TestInsertionAndDeletion<CTrie<int, char*> >(1, 4, 2, 5, 3);
// KDTree
		TestConstructor<CKDTree<Deku2D::Vector2, int>>();
//////////////////////////////////////////////////////////////////////////
// Suddenly, a non uniform in place test appears:
/* Commented while working on KDTree
		char * Strings[8] = 
		{
			"a",
			"ab",
			"abcdef",
			"",
			"bcdef",
			"bcdah",
			"a",
			"ab",
		};
		CTrie<char*, int> TestTrie;
		typedef CTrie<char*, int> C;
		for(unsigned int i = 0; i < 8; ++i)
			TestTrie.insert(std::make_pair(Strings[i], i));
		CHECK(TestTrie.size() == 6);
		cout << endl;
		for(CTrie<char*, int>::iterator TestTrieIterator(&TestTrie); TestTrieIterator.Ok(); ++--++TestTrieIterator)
			cout << (*TestTrieIterator).first << " -- " << (*TestTrieIterator).second << endl;


//======// Special performance tests for Tries realizations and some assoc containers here
		cout << DELIMITER;
		cout << "Performance testing:" << endl;
 		PerformanceTestSequence<CArray<int, CDynAllocator<int, 2, 4>>>();
 		PerformanceTestSequence<CArray<string, CDynAllocator<string, 2, 4>>>();
		PerformanceTestSequence<CArray<CTestClass, CDynAllocator<CTestClass, 2, 4>>>();
 		PerformanceTestSequence<CList<int>>();
 		PerformanceTestSequence<CList<string>>();
		PerformanceTestSequence<CList<CTestClass>>();
		const string TestWordsSources[9] = 
		{
			"Tests\\length02.txt",
			"Tests\\length03.txt",
			"Tests\\length04.txt",
			"Tests\\length05.txt",
			"Tests\\length06.txt",
			"Tests\\length07.txt",
			"Tests\\length08.txt",
			"Tests\\dictionary.txt",
			//"Tests\\Russian-English.dic",
			"Tests\\enrus.dic"	// DANGER: RUSSIAN TEXT: TRIE EATS ALL 
			//	THE MEMORY AVAILABLE (at least 2Gb on my PC and you get
			//	"nice"-memory-management-in-WinXP results after)
		};
		CArray<string> InputWords;
		for(unsigned int i = 0; i < 8; i++)	// Ommiting some hard tests from the end;
		{
			cout << DELIMITER_SMALL;
			GetFileContentsParsedToWords(InputWords, (TestWordsSources[i]).c_str());
			PerformanceTest<CTrie<string, int>>(InputWords);
			PerformanceTest<std::map<string, int>>(InputWords);
// 			PerformanceTest<CTrieBad<string, int>>(InputWords);	// Incompatible interface at some minor places
// 			PerformanceTest<CTreeMap<string, int>>(InputWords);	// Same
// 			PerformanceTest<CSkipList<string, int>>(InputWords); // Same
		}
		//	А вообще неоптимизированный по памяти Trie на больших (по сути - 
		//	не таких уж и больших) тестах столько памяти сжирает, что страшно.
		//	Да и к тому же на больших тестах тормозит очень страшно именно 
		//	из-за хреновой аллокации. (С одной стороны хреновой, с другой - просто много
		//	оно поидее в pagefile лезть активно начинает, когда запредельно всё это становится.)
		//	Надо делать пул для памяти и вообще Burst trie писать...
*/
		cout << DELIMITER;
//======// Some *very* special tests, added and deleted by developer on the fly:
// 		typedef CKDTree<Vector2, int> our_kdtree;
// 		typedef our_kdtree C;
// 		vector<CKDTree<Vector2, int>::ValueTypeNonConst> Points;
// 		using std::make_pair;
// 		Points.push_back(make_pair(Vector2(2, 3), 0));
// 		Points.push_back(make_pair(Vector2(5, 4), 1));
// 		Points.push_back(make_pair(Vector2(9, 6), 2));
// 		Points.push_back(make_pair(Vector2(4, 7), 3));
// 		Points.push_back(make_pair(Vector2(8, 1), 4));
// 		Points.push_back(make_pair(Vector2(7, 2), 5));
// 		CKDTree<Vector2, int> TestKDTree(Points, 2);
// 		CHECK(TestKDTree.size() == 6);
// 
// 		CHECK(!TestKDTree.find(Vector2(2, 4.5f)).Ok());
// 
// 		TestKDTree[Vector2(2, 3)] = 10;		
// 		CHECK(TestKDTree[Vector2(2, 3)] == 10);
// 
// 		TestKDTree.erase(Vector2(7, 2));
// 		TestKDTree.erase(Vector2(2, 3));
// 		TestKDTree.erase(Vector2(5, 4));
// 		CHECK(TestKDTree.size() == 3);
// 
// 		try
// 		{
// 			for(our_kdtree::iterator i = TestKDTree.begin(); i.Ok(); ++i)
// 				(*i);
// 			for(our_kdtree::iterator i = TestKDTree.end(); i.Ok(); --i)
// 				(*i);
// 			CHECK(true);
// 		}
// 		catch(...)
// 		{
// 			CHECK(false);
// 		}
// 
// 		KDTreePerfTest<CKDTree<Vector2, int>>();

// 		PerformanceTestSequence<dtl::vector<int, CDynAllocator<int, 2, 4>>>();
// 		PerformanceTestSequence<dtl::vector<string, CDynAllocator<string, 2, 4>>>();
// 		//PerformanceTestSequence<dtl::vector<CTestClass, CDynAllocator<CTestClass, 2, 4>>>();
// 		cout << DELIMITER_SMALL << endl;
// 		PerformanceTestSequence<std::vector<int>>();
// 		PerformanceTestSequence<std::vector<string>>();
// 		//PerformanceTestSequence<std::vector<CTestClass>>();

//////////////////////////////////////////////////////////////////////////
// string tests

		dtl::string *test = new dtl::string("abc");
		typedef dtl::string C;
		CHECK((*test) == "abc");
		dtl::string *testFTW = new dtl::string("");
		*testFTW = *test;
		CHECK((*testFTW) == "abc");
		cout << *test << " " << *testFTW;
		*test += *test;
		CHECK((*test) == "abcabc");
		cout << endl << *test << endl;
		std::cin >> *test;
		cout << "\n" << *test;

		char& rc = (*test)[0];  // take a reference to the first char
		dtl::string *s2 = new dtl::string(""); 
		*s2 = *test;// take a copy of the string
		rc = 'x';                  // oops: also modifies s2!
		CHECK((*test)[0] != (*s2)[0]);

		delete s2;
		delete testFTW;
		delete test;
	}
	catch(const exception& e)
	{
		cout << e.what() << endl;
		return EXIT_FAILURE;
	}
	catch(const char* e)
	{
		cout << e << endl;
		return EXIT_FAILURE;
	}
	PrintTotals();
/*

	std::string TestString = "I thik that string is long enough for me.";
	size_t StringSize = sizeof(std::string);
	StringSize = sizeof("aaaaaaa");
	StringSize = sizeof('a');
	StringSize = sizeof(TestString);
	StringSize = sizeof(&TestString);
	StringSize = sizeof(std::string("aaaaaaa"));
	return EXIT_SUCCESS;
*/
}