#pragma once

#include "ClassInfo.h"
#include "PropertyInfo.h"
#include "Util.h"
#include "Test.h"

//D2D_DECL_CLASS_INFO(float)

//D2D_DECL_CLASS_INFO(Vector2)
//D2D_DECL_CLASS_PROPERTY(Vector2, float, x)

//D2D_DECL_CLASS_INFO(CFoo)
//D2D_DECL_CLASS_INFO(CBar)
//D2D_DECL_CLASS_INFO(CAFoo)
//D2D_DECL_CLASS_INFO(CTFoo)

D2D_DECL_CLASS_INFO(float)

//D2D_DECL_CLASS_INFO(Vector2*)

D2D_DECL_CLASS_INFO(Vector2)
	D2D_DECL_OWNED_PROPERTY_INFO(Vector2, float, X)
	D2D_DECL_OWNED_PROPERTY_INFO(Vector2, float, Y)

D2D_DECL_CLASS_INFO(CFoo)
	D2D_DECL_OWNED_PROPERTY_INFO(CFoo, Vector2, Position)
	D2D_DECL_OWNED_PROPERTY_INFO(CFoo, bool, Flag)
	D2D_DECL_OWNED_PROPERTY_INFO(CFoo, string, Str)
	D2D_DECL_OWNED_ARRAY_PROPERTY_INFO_EX(CFoo, float, Array, PushToArray, GetArray, ArraySize)
	//D2D_DECL_OWNED_PROPERTY_INFO(CFoo, Vector2*, PtrPosition)

D2D_DECL_DERIVED_CLASS_INFO(CBarDerived, CFoo)
	D2D_DECL_OWNED_PROPERTY_INFO(CBarDerived, int, H)
