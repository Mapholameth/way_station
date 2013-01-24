#include "dtl_Test.h"

#include <vector>

unsigned int TotalTests(0);
unsigned int PassedTests(0);
unsigned int Numberer(0);
dtl::vector<string> FailedTestsInfo;

void Check(bool Ok, const string &ContainerInfo, int Line, const string &FunctionName)
{
	PassedTests += Ok;
	TotalTests++;
	if (!Ok)
	{
		FailedTestsInfo.push_back("Function: " + FunctionName + ", Line: " + to_string(Line) + ", with container type:");
		FailedTestsInfo.push_back("\t" + ContainerInfo);
	}
}

//#define TEST_CLASS_THROWS_EXCEPTION
const unsigned int CTEST_ALLOCATION_SIZE = 1024;

CTestClass::CTestClass() : SomeData(NULL), StringMember(""), IntegralMember(Numberer++)
{
	try
	{
		SomeData = malloc(CTEST_ALLOCATION_SIZE);	// alloc 1 mbytes;
#ifdef TEST_CLASS_THROWS_EXCEPTION
		if (IntegralMember == 4)
			throw std::bad_exception();
#endif // TEST_CLASS_THROWS_EXCEPTION
	}
	catch(...)
	{
		free(SomeData);
		throw;
	}
}

CTestClass::~CTestClass()
{
	free(SomeData);
	SomeData = NULL;
}

bool CTestClass::operator <(const CTestClass &Operand) const
{
	return IntegralMember < Operand.IntegralMember;
}

CTestClass::CTestClass(const CTestClass &TestClass) : SomeData(NULL),
IntegralMember(TestClass.IntegralMember), StringMember(TestClass.StringMember)
{
	SomeData = malloc(CTEST_ALLOCATION_SIZE);
	memcpy(SomeData, TestClass.SomeData, CTEST_ALLOCATION_SIZE);
}

CTestClass& CTestClass::operator =(const CTestClass &Operand)
{
	IntegralMember = Operand.IntegralMember;
	StringMember = Operand.StringMember;
	memcpy(SomeData, Operand.SomeData, CTEST_ALLOCATION_SIZE);
	return *this;
}

bool CTestClass::operator ==(const CTestClass &Operand) const
{
	return IntegralMember == Operand.IntegralMember;
}

bool CTestClass::operator !=(const CTestClass &Operand) const
{
	return IntegralMember != Operand.IntegralMember;
}

CTestClass& CTestClass::operator +=(const CTestClass &Operand)
{
	this->IntegralMember = Numberer++;
	return *this;
}

ostream& operator << (ostream &os, const CTestClass &Source)
{
	os << "Integral member: " << Source.IntegralMember << ", String member: " << Source.StringMember << endl;
	return os;
}

void PrintTotals()
{
	cout << DELIMITER;
	cout << "Summary:" << endl;
	cout << "\tTotal tests: " << TotalTests << endl;
	cout << "\tPassed tests: " << PassedTests << endl;
	if (FailedTestsInfo.size() > 0)
	{
		cout << "\tFailed tests: " << TotalTests - PassedTests << endl;
		cout << "\tFailed tests are:" << endl;
	}
	for(dtl::vector<string>::iterator i = FailedTestsInfo.begin(); i != FailedTestsInfo.end(); i++)
		cout << "\t" << *i << endl;
}

void GetFileContentsParsedToWords(dtl::vector<string> &Array, const char *Filename)
{
	FILE *ifile = fopen(Filename, "rb");
	if (ifile  == NULL)
		throw("bad");

	struct stat FileStat;
	if (stat(Filename, &FileStat))
		throw("bad");

	char *data = NULL;
	data  = new char [FileStat.st_size  + 1];
	data [FileStat.st_size] = 0;
	unsigned int BytesRead = fread(data, sizeof(char), FileStat.st_size, ifile);
	if (BytesRead != static_cast<unsigned int>(FileStat.st_size))
		throw("bad");
	Array.clear();
	fclose(ifile);
	string temp;
	unsigned int i = 0, i0, wcount = 0;
	while (data[i] != 0)
	{
		i0 = i;
		while(data[i] != '\n' && data[i] != '\r' && data[i] != ' ' && data[i] != 0 && data[i] != '\t')
			i++;
		temp.append(data + i0, i - i0);
		while(data[i] == '\n' || data[i] == '\r' || data[i] == ' ' || data[i] == '\t')
			i++;
		//Array.push_back(temp);
		Array[wcount++] = temp;
		temp.clear();
	}
	delete [] data;
}
