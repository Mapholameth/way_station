#pragma once

#include <ostream>
#include <vector>
#include <string>
using namespace std;

class ISerializer
{
public:
	template<typename TProperty>
	static void Serialize(ostream &out, const TProperty* property, const typename TProperty::TOwner* owner)
	{

	}
};

class CStreamSerializer
{
public:
	template<typename T>
	static void Serialize(ostream &out, const string& name, const T& value)
	{
		out << name << " = '" << value << "';";
	}

	template<typename T>
	static void Deserialize(istream &out, const string& name, T& value )
	{
		name;
		out >> value;
	}

	template<>
	static void Serialize<vector<int>>(ostream &out, const string& name, const vector<int>& value)
	{
		out << name << " = '";
		for (unsigned i = 0; i < value.size(); i++ )
			out << value[i] << " ,";
		out << "';";
	}

	template<>
	static void Deserialize<vector<int>>(istream &out, const string& name, vector<int>& value )
	{
		name;	// unreferenced formal parameter warning
		for( unsigned i = 0; i < value.size(); i++ )
			out >> value[i];
	}
};