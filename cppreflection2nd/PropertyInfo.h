#pragma once

class PropertyInfo
{
public:
	virtual char* Name() const = 0;
	virtual char* TypeName() const = 0;
	virtual char* OwnerName() const = 0;
};

#define D2D_DECL_OWNED_PROPERTY_INFO(OWNER, TYPE, NAME)	\
class OwnedPropertyInfo##OWNER##NAME : public PropertyInfo	\
{	\
public:	\
	OwnedPropertyInfo##OWNER##NAME()	\
{	\
	ClassInfo::_classInfos[#OWNER]->_props[#NAME] = &_instance;	\
}	\
	virtual char* Name() const { return #NAME; }	\
	virtual char* TypeName() const { return #TYPE; }	\
	virtual char* OwnerName() const { return #OWNER; }	\
	static OwnedPropertyInfo##OWNER##NAME _instance;	\
};	\
OwnedPropertyInfo##OWNER##NAME OwnedPropertyInfo##OWNER##NAME::_instance;	\


