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
	static map<string, ClassInfo*> _classInfos;
	static map<string, PropertyInfo*> _props;
};
map<string, ClassInfo*> ClassInfo::_classInfos;
map<string, PropertyInfo*> ClassInfo::_props;

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
		static TypedClassInfo##CLASS_NAME _instance;	\
	};	\
	TypedClassInfo##CLASS_NAME TypedClassInfo##CLASS_NAME::_instance;	\




//static CLASS_NAME* New() { return Make<CLASS_NAME>::New(); }	\

