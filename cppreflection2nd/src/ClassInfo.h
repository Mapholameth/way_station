#pragma once

#include "PropertyInfo.h"
#include <map>
#include <string>
using std::map;
using std::string;

class ClassInfo
{
public:
	virtual char* Name() const = 0;
	virtual void* New() const = 0;
	virtual PropertyInfo* GetProperty(const string &name) const = 0;
	virtual map<string, PropertyInfo*>& Properties() = 0;
	virtual ClassInfo* BaseInfo() = 0;
	static map<string, ClassInfo*> _classInfos;
};
map<string, ClassInfo*> ClassInfo::_classInfos;

#define D2D_DECL_CLASS_INFO(CLASS_NAME)	\
	class TypedClassInfo##CLASS_NAME : public ClassInfo	\
	{	\
	public:	\
		TypedClassInfo##CLASS_NAME()	\
		{	\
			_classInfos[#CLASS_NAME] = &_instance;	\
		}	\
		virtual void* New() const { return Make<CLASS_NAME>::New(); }	\
		char* Name() const { return #CLASS_NAME; }	\
		PropertyInfo* GetProperty(const string &name) const { return _props[name]; }	\
		map<string, PropertyInfo*>& Properties() { return _props; }	\
		ClassInfo* BaseInfo() { return 0; }	\
		static TypedClassInfo##CLASS_NAME _instance;	\
		static map<string, PropertyInfo*> _props;	\
	};	\
	TypedClassInfo##CLASS_NAME TypedClassInfo##CLASS_NAME::_instance;	\
	map<string, PropertyInfo*> TypedClassInfo##CLASS_NAME::_props;	\

#define D2D_DECL_DERIVED_CLASS_INFO(CLASS_NAME, BASE_NAME)	\
	class TypedClassInfo##CLASS_NAME : public ClassInfo	\
	{	\
	public:	\
		TypedClassInfo##CLASS_NAME()	\
		{	\
			_classInfos[#CLASS_NAME] = &_instance;	\
		}	\
		virtual void* New() const { return Make<CLASS_NAME>::New(); }	\
		char* Name() const { return #CLASS_NAME; }	\
		PropertyInfo* GetProperty(const string &name) const { return _props[name]; }	\
		ClassInfo* BaseInfo() { return _classInfos[#BASE_NAME]; }	\
		map<string, PropertyInfo*>& Properties() { return _props; }	\
		static TypedClassInfo##CLASS_NAME _instance;	\
		static map<string, PropertyInfo*> _props;	\
	};	\
	TypedClassInfo##CLASS_NAME TypedClassInfo##CLASS_NAME::_instance;	\
	map<string, PropertyInfo*> TypedClassInfo##CLASS_NAME::_props;	\


//static CLASS_NAME* New() { return Make<CLASS_NAME>::New(); }	\
