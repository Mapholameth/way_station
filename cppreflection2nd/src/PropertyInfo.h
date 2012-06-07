#pragma once

#include <string>
using std::string;
#include "Util.h"

class PropertyInfo
{
public:
	virtual char* Name() const = 0;
	virtual char* TypeName() const = 0;
	virtual char* OwnerName() const = 0;
	virtual void SetString(void* owner, const string &value) = 0;
	virtual void SetValue(void* owner, void *property) = 0;
	virtual string GetString(void* owner) = 0;
	virtual bool Integral() = 0;
};

#define D2D_DECL_OWNED_PROPERTY_INFO(OWNER, TYPE, NAME)	\
class OwnedPropertyInfo##OWNER##NAME : public PropertyInfo	\
{	\
public:	\
	OwnedPropertyInfo##OWNER##NAME()	\
{	\
	ClassInfo::_classInfos[#OWNER]->Properties()[#NAME] = &_instance;	\
}	\
	virtual char* Name() const { return #NAME; }	\
	virtual char* TypeName() const { return #TYPE; }	\
	virtual char* OwnerName() const { return #OWNER; }	\
	virtual void SetString(void* owner, const string &value)	\
	{	\
		OWNER *typedOwner = static_cast<OWNER*>(owner);	\
		typedOwner->Set##NAME(From<TYPE>::String(value));	\
	}	\
	virtual void SetValue(void* owner, void *property)	\
	{	\
		OWNER *typedOwner = static_cast<OWNER*>(owner);	\
		TYPE *typedProperty = static_cast<TYPE*>(property);	\
		typedOwner->Set##NAME(*typedProperty);	\
	}	\
	string GetString(void* owner)	\
	{	\
		OWNER *typedOwner = static_cast<OWNER*>(owner);	\
		return To<TYPE>::String(typedOwner->Get##NAME());	\
	}	\
	bool Integral()	\
	{	\
		return IsIntegral<TYPE>::result;	\
	}	\
	static OwnedPropertyInfo##OWNER##NAME _instance;	\
};	\
OwnedPropertyInfo##OWNER##NAME OwnedPropertyInfo##OWNER##NAME::_instance;	\

