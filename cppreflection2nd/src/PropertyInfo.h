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
	virtual void SetString(void *owner, const string &value) = 0;	// put into class info
	virtual void SetValue(void *owner, void *property) = 0;
	virtual void* GetValue(void *owner) = 0;
	virtual string GetString(void* owner) = 0;	// put into class info
	virtual bool Integral() = 0;
	virtual bool IsArray() = 0;
	virtual void PushValue(void *owner, void *value) = 0;
	virtual unsigned GetArraySize(void *owner) = 0;
	virtual void* GetValue(void *owner, unsigned index) = 0;
};

#define D2D_DECL_OWNED_PROPERTY_INFO_EX(OWNER, TYPE, NAME, SETTER, GETTER)	\
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
		typedOwner->SETTER(From<TYPE>::String(value));	\
	}	\
	virtual void SetValue(void* owner, void *property)	\
	{	\
		OWNER *typedOwner = static_cast<OWNER*>(owner);	\
		TYPE *typedProperty = static_cast<TYPE*>(property);	\
		typedOwner->SETTER(*typedProperty);	\
	}	\
	void* GetValue(void *owner)	\
	{	\
		TYPE *value = new TYPE;	\
		*value = static_cast<OWNER*>(owner)->GETTER();	\
		return value;	\
	}	\
	string GetString(void* owner)	\
	{	\
		OWNER *typedOwner = static_cast<OWNER*>(owner);	\
		return To<TYPE>::String(typedOwner->GETTER());	\
	}	\
	bool Integral()	\
	{	\
		return IsIntegral<TYPE>::result;	\
	}	\
	bool IsArray()	\
	{	\
		return false;	\
	}	\
	void PushValue(void *owner, void *value) { throw "pizdec"; }	\
	unsigned GetArraySize(void *owner) { throw "pizdec"; }	\
	void* GetValue(void *owner, unsigned index) { throw "pizdec"; }	\
	static OwnedPropertyInfo##OWNER##NAME _instance;	\
};	\
OwnedPropertyInfo##OWNER##NAME OwnedPropertyInfo##OWNER##NAME::_instance;	\

#define D2D_DECL_OWNED_PROPERTY_INFO(OWNER, TYPE, NAME)	\
	D2D_DECL_OWNED_PROPERTY_INFO_EX(OWNER, TYPE, NAME, Set##NAME, Get##NAME)	\

#define D2D_DECL_OWNED_ARRAY_PROPERTY_INFO_EX(OWNER, TYPE, NAME, PUSHER, GETTER, SIZEGETTER)	\
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
		throw("pizdec");	\
	}	\
	virtual void SetValue(void* owner, void *property)	\
	{	\
		throw("pizdec");	\
	}	\
	void* GetValue(void *owner) { throw "pizdec"; };	\
	string GetString(void* owner)	\
	{	\
		throw("pizdec");	\
	}	\
	bool Integral()	\
	{	\
		return false;	\
	}	\
	bool IsArray()	\
	{	\
		return true;	\
	}	\
	void PushValue(void *owner, void *value)	\
	{	\
		static_cast<OWNER*>(owner)->PUSHER(*(static_cast<TYPE*>(value)));	\
	}	\
	unsigned GetArraySize(void *owner)	\
	{	\
		return static_cast<OWNER*>(owner)->SIZEGETTER();	\
	}	\
	void* GetValue(void *owner, unsigned index)	\
	{	\
		TYPE *value = new TYPE;	\
		*value = static_cast<OWNER*>(owner)->GETTER(index);	\
		return value;	\
	}	\
	static OwnedPropertyInfo##OWNER##NAME _instance;	\
};	\
OwnedPropertyInfo##OWNER##NAME OwnedPropertyInfo##OWNER##NAME::_instance;	\

