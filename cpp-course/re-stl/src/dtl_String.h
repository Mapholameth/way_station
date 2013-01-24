#ifndef	_DTL_STRING_H_
#define _DTL_STRING_H_

#include "dtl_Allocator.h"
#include "dtl_Array.h"
#include <ostream>
#include <istream>
#include <algorithm>
#include <limits>

namespace dtl
{
	class string
	{
	private:
		class string_storage
		{
			friend class string;
		private:
			size_t Allocated;
			size_t Length;
			size_t RefCount;
			char *Data;

		public:
			string_storage(const string_storage &AStorage);
			string_storage();
			string_storage(const char * ACharachters);
			string_storage(size_t n, char fill);
			void Reserve(size_t n);
			~string_storage();
		};

		string_storage *Storage;

		void _IncRef();
		void _DecRef();
		void _MakeUnique();
		void _ResizeStorage(size_t NewSize);
		void _AboutToModify(size_t n, bool MarkUnshareable = false);

	public:
		static const size_t npos = -1;
		static size_t unshareable;

		typedef char* iterator;
		typedef char value_type;

		string();
		string(const string &AString);
		string(const char *ACharacters = "");
		~string();
		
		string& operator =(const string &rhs);
		string& operator =(const char *rhs);
		string& operator =(char rhs);

		iterator begin();
		iterator end();

		size_t size() const;
		size_t length() const;
		size_t capacity() const;
		void clear();
		bool empty() const;

		char operator [](size_t pos) const;
		char& operator [](size_t pos);
		char at(size_t pos) const;
		char& at(size_t pos);

		string& operator +=(const string& str);
		string& operator +=(const char* s);
		string& operator +=(char c);
		string& append(const string& str);
		string& append(const char* s);
		string& append(size_t n, char c);

// 		string& insert(size_t pos1, const string& str);
// 		string& insert(size_t pos1, const string& str, size_t pos2, size_t n);
// 		string& insert(size_t pos1, const char* s, size_t n);
// 		string& insert(size_t pos1, const char* s);
// 		string& insert(size_t pos1, size_t n, char c);
// 		iterator insert(iterator p, char c);
// 		void insert(iterator p, size_t n, char c);
// 		template<class InputIterator>
// 		void insert(iterator p, InputIterator first, InputIterator last);
// 
// 		string& erase(size_t pos = 0, size_t n = npos);
// 		iterator erase(iterator position);
// 		iterator erase(iterator first, iterator last);
// 
// 		string& replace(size_t pos1, size_t n1,   const string& str);
// 		string& replace(iterator i1, iterator i2, const string& str);
// 
// 		string& replace(size_t pos1, size_t n1, const string& str, size_t pos2, size_t n2);
// 
// 		string& replace(size_t pos1, size_t n1,   const char* s, size_t n2);
// 		string& replace(iterator i1, iterator i2, const char* s, size_t n2);
// 
// 		string& replace(size_t pos1, size_t n1,   const char* s);
// 		string& replace(iterator i1, iterator i2, const char* s);
// 
// 		string& replace(size_t pos1, size_t n1,   size_t n2, char c);
// 		string& replace(iterator i1, iterator i2, size_t n2, char c);
// 
// 		template<class InputIterator>
// 		string& replace(iterator i1, iterator i2, InputIterator j1, InputIterator j2);

//		void swap(string& str);

		const char* c_str() const;

//		string substr(size_t pos = 0, size_t n = npos) const;

		int compare(const string& str) const;
		int compare(const char* s) const;

//		size_t copy(char* s, size_t n, size_t pos = 0) const;

// 		size_t find(const string& str, size_t pos = 0) const;
// 		size_t find(const char* s, size_t pos, size_t n) const;
// 		size_t find(const char* s, size_t pos = 0) const;
// 		size_t find(char c, size_t pos = 0) const;
// 
// 		size_t rfind(const string& str, size_t pos = npos) const;
// 		size_t rfind(const char* s, size_t pos, size_t n) const;
// 		size_t rfind(const char* s, size_t pos = npos) const;
// 		size_t rfind(char c, size_t pos = npos) const;
// 
// 		size_t find_first_of(const string& str, size_t pos = 0) const;
// 		size_t find_first_of(const char* s, size_t pos, size_t n) const;
// 		size_t find_first_of(const char* s, size_t pos = 0) const;
// 		size_t find_first_of(char c, size_t pos = 0) const;
// 
// 		size_t find_last_of(const string& str, size_t pos = npos) const;
// 		size_t find_last_of(const char* s, size_t pos, size_t n) const;
// 		size_t find_last_of(const char* s, size_t pos = npos) const;
// 		size_t find_last_of(char c, size_t pos = npos) const;
// 
// 		size_t find_first_not_of(const string& str, size_t pos = 0) const;
// 		size_t find_first_not_of(const char* s, size_t pos, size_t n) const;
// 		size_t find_first_not_of(const char* s, size_t pos = 0) const;
// 		size_t find_first_not_of(char c, size_t pos = 0) const;
// 
// 		size_t find_last_not_of(const string& str, size_t pos = npos) const;
// 		size_t find_last_not_of(const char* s, size_t pos, size_t n) const;
// 		size_t find_last_not_of(const char* s, size_t pos = npos) const;
// 		size_t find_last_not_of(char c, size_t pos = npos) const;

		friend string operator +(const string& lhs, const string& rhs)
		{
			string temp = lhs;
			temp += rhs;
			return temp;
		}

		friend string operator +(const char* lhs, const string& rhs)
		{
			string temp = lhs;
			temp += rhs;
			return temp;
		}

		friend string operator +(char lhs, const string& rhs)
		{
			string temp = rhs;
			temp += lhs;
			return temp;
		}

		friend string operator +(const string& lhs, const char* rhs)
		{
			string temp = lhs;
			temp += rhs;
			return temp;
		}

		friend string operator +(const string& lhs, char rhs)
		{
			string temp = lhs;
			temp += rhs;
			return temp;
		}
		
		friend bool operator ==(const string& lhs, const string& rhs)
		{
			return lhs.compare(rhs);
		}

		friend bool operator ==(const char* lhs, const string& rhs)
		{
			return rhs.compare(lhs) == 0;
		}

		friend bool operator ==(const string& lhs, const char* rhs)
		{
			return lhs.compare(rhs) == 0;
		}

		friend bool operator !=(const string& lhs, const string& rhs)
		{
			return !(lhs == rhs);
		}

		friend bool operator !=(const char* lhs, const string& rhs)
		{
			return !(lhs == rhs);
		}

		friend bool operator !=(const string& lhs, const char* rhs)
		{
			return !(lhs == rhs);
		}

		friend bool operator <(const string& lhs, const string& rhs)
		{
			return lhs.compare(rhs) < 0;
		}

		friend bool operator <(const char* lhs, const string& rhs)
		{
			return rhs.compare(lhs) >= 0;
		}

		friend bool operator <(const string& lhs, const char* rhs)
		{
			return lhs.compare(rhs) < 0;
		}

		friend bool operator >(const string& lhs, const string& rhs)
		{
			return lhs.compare(rhs) > 0;
		}

		friend bool operator >(const char* lhs, const string& rhs)
		{
			
			return rhs.compare(lhs) <= 0;
		}

		friend bool operator >(const string& lhs, const char* rhs)
		{
			return lhs.compare(rhs);
		}

		friend bool operator <=(const string& lhs, const string& rhs)
		{
			return lhs.compare(rhs) <= 0;
		}

		friend bool operator <=(const char* lhs, const string& rhs)
		{
			return rhs.compare(lhs) > 0;
		}

		friend bool operator <=(const string& lhs, const char* rhs)
		{
			return lhs.compare(rhs);
		}

		friend bool operator >=(const string& lhs, const string& rhs)
		{
			return lhs.compare(rhs) >= 0;
		}

		friend bool operator >=(const char* lhs, const string& rhs)
		{
			return rhs.compare(lhs) < 0;
		}

		friend bool operator >=(const string& lhs, const char* rhs)
		{
			return lhs.compare(rhs) >= 0;
		}


		friend std::ostream& operator <<(std::ostream& os, const string& str);
		friend std::istream& operator >>(std::istream& is, string& str);
	};
};

#endif // _DTL_STRING_H_
