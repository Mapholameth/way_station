#include "dtl_String.h"

namespace dtl
{
//////////////////////////////////////////////////////////////////////////
// Storage

string::string_storage::string_storage(const char * ACharachters) : RefCount(1), Length(0), Allocated(0), Data(NULL)
{
	Reserve(strlen(ACharachters) + 1);
	Length = strlen(ACharachters);
	strncpy(Data, ACharachters, Length + 1);
	Data[Length] = 0;
}

string::string_storage::string_storage(size_t n, char fill) : RefCount(1), Length(0), Allocated(0), Data(NULL)
{
	Reserve(n + 1);
	Length = n;
	Data = new char[Length];
	memset(Data, fill, n);
	Data[n] = 0;
}

string::string_storage::string_storage() : Allocated(0), Length(0), RefCount(1), Data(NULL)
{

}

string::string_storage::string_storage( const string_storage &AStorage ) : Allocated(0), Length(0), RefCount(1), Data(NULL)
{
	Reserve(AStorage.Length);
	Length = AStorage.Length;
	memcpy(Data, AStorage.Data, AStorage.Length);
	Data[Length] = 0;
}

string::string_storage::~string_storage()
{
	delete [] Data;
}

void string::string_storage::Reserve(size_t n)
{
	if(Length < n)
	{
		size_t newlen = std::max<size_t>(Length * 1.5, n);
		char *NewData = new char[newlen];
		memcpy(NewData, Data, Length);

		delete[] Data;
		Data = NewData;
		Allocated = newlen;
	}
}
//////////////////////////////////////////////////////////////////////////
// string

void string::_IncRef()
{
	if (Storage == NULL)
		Storage = new string_storage("");
	else
		Storage->RefCount++;
}

void string::_DecRef()
{
	if (Storage == NULL)
		return;
	if (Storage->RefCount == unshareable || --Storage->RefCount < 1)
	{
		delete Storage;
		Storage = NULL;
	}
}

void string::_MakeUnique()
{
	if (Storage == 0)
		return;
	if (Storage->RefCount <= 1)
		return;
	string_storage *temp_storage = new string_storage(Storage->Data);
	_DecRef();
	Storage = temp_storage;	
}


void string::_ResizeStorage(size_t NewSize)
{
	_MakeUnique();
	if (NewSize == 0)
	{
		delete Storage;
		Storage = NULL;
		return;
	}
	if (Storage == NULL)
	{
		if (NewSize == 0)
			return;
		Storage = new string_storage();
	}
	Storage->Reserve(NewSize);
}

void string::_AboutToModify(size_t n, bool MarkUnshareable)
{
	if(Storage->RefCount > 1 && Storage->RefCount != unshareable)
	{
		std::auto_ptr<string_storage> NewData(new string_storage);
		NewData.get()->Reserve(std::max(Storage->Allocated, n));
		memcpy(NewData.get()->Data, Storage->Data, Storage->Length);
		NewData.get()->Length = Storage->Length;

		Storage->RefCount--;
		Storage = NewData.release();
	}
	else
	{
		Storage->Reserve(n);
	}
	Storage->RefCount = MarkUnshareable ? unshareable : 1;
}

string::string() : Storage(NULL)
{

}

string::string(const string &AString)
{
	if (AString.Storage->RefCount != unshareable)
	{
		Storage = AString.Storage;
		_IncRef();
	}
	else
		Storage = new string_storage(*AString.Storage);
}

string::string(const char *ACharacters /*= ""*/) : Storage(new string_storage(ACharacters))
{

}

string::~string()
{
	_DecRef();
}

string& string::operator =(const string &rhs)
{
	_DecRef();
	if (rhs.Storage->RefCount != unshareable)
	{
		Storage = rhs.Storage;
		_IncRef();
	}
	else
		Storage = new string_storage(*rhs.Storage);
	return *this;
}

string& string::operator =(const char *rhs)
{
	_DecRef();
	Storage = new string_storage(rhs);
	return *this;
}

string& string::operator =(char rhs)
{
	_DecRef();
	Storage = new string_storage(1, rhs);
	return *this;
}

size_t string::size() const
{
	return Storage->Length;
}

size_t string::length() const
{
	return Storage->Length;
}

bool string::empty() const
{
	return Storage == NULL || Storage->Length == 0;
}

dtl::string::iterator string::begin()
{
	return Storage->Data;
}

dtl::string::iterator string::end()
{
	return Storage->Data + Storage->Length;
}

size_t string::capacity() const
{
	return Storage->Allocated;
}

void string::clear()
{
	_DecRef();
	Storage = NULL;
}

char string::operator[](size_t pos) const
{
	assert(Storage != NULL && pos < Storage->Length);
	return Storage->Data[pos];
}

char& string::operator[](size_t pos)
{
	assert(Storage != NULL && pos < Storage->Length);
	//_MakeUnique();
	_AboutToModify(Storage->Length, true);
	return Storage->Data[pos];
}

char string::at(size_t pos) const
{
	assert(Storage != NULL);
	if (pos > size())
		throw std::out_of_range("dtl::string out of range");
	return Storage->Data[pos];
}

char& string::at(size_t pos)
{
	assert(Storage != NULL);
	if (pos > size())
		throw std::out_of_range("dtl::string out of range");
	_MakeUnique();
	return Storage->Data[pos];
}

const char* string::c_str() const
{
	assert(Storage != NULL);
	return Storage->Data;
}

int string::compare( const string& str ) const
{
	return stricmp(Storage->Data, str.Storage->Data);
}

int string::compare( const char* s ) const
{
	return stricmp(Storage->Data, s);
}

string& string::operator+=( const string& str )
{
	return this->append(str);
}

string& string::operator+=( const char* s )
{
	return this->append(s);
}

string& string::operator+=( char c )
{
	//string temp = string(c);
	return *this;
}

string& string::append( const string& str )
{
	_MakeUnique();
	size_t NewSize = (Storage == 0 ? 0 : (Storage->Length)) + str.length() + 1;
	_ResizeStorage(NewSize);
	memcpy(Storage->Data + Storage->Length, str.Storage->Data, str.Storage->Length);
	Storage->Length = NewSize - 1;
	Storage->Data[Storage->Length] = 0;
	return *this;
}

string& string::append( const char* s )
{
	_MakeUnique();
	size_t NewSize = (Storage == 0 ? 0 : (Storage->Length)) + strlen(s) + 1;
	_ResizeStorage(NewSize);
	memcpy(Storage->Data + Storage->Length, s, strlen(s));
	Storage->Length = NewSize - 1;
	Storage->Data[Storage->Length] = 0;
	return *this;
}

string& string::append( size_t n, char c )
{
	_MakeUnique();
	// string temp(n, c);
	// *this += temp;
	// and do something
	return *this;
}
size_t string::unshareable = std::numeric_limits<unsigned>::max();

}

//////////////////////////////////////////////////////////////////////////
// Global overloads

std::ostream& dtl::operator <<(std::ostream& os, const dtl::string& str)
{
	os << str.c_str();
	return os;
}

std::istream& dtl::operator >>(std::istream& is, dtl::string& str)
{
#define BLOCK_SIZE 3 /*1024*/
char *buffer = new char[BLOCK_SIZE];
	int k;
	str.clear();
	while (is.good())
	{
		is.get(buffer, BLOCK_SIZE);
		k = is.gcount();
		int i = 0;
		while (i < k && buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\n' && buffer[i] != '\r')
			i++;
		buffer[i] = 0;
		str += buffer;
	}
	delete [] buffer;
	return is;
}
