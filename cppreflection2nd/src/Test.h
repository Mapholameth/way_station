#pragma once

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class Vector2
{
public:
	Vector2(){}
	Vector2(float x, float y) : _x(x), _y(y) {}
	void SetX(const float &x) { _x = x; }
	void SetY(const float &y) { _y = y; }	
	float GetX() const { return _x; }
	float GetY() const { return _y; }

private:
	float _x;
	float _y;
};

class CFoo
{
	D2D_INJECT_TYPE_INFO(CFoo)

public:
	Vector2 GetPosition() const { return _position; }
	void SetPosition(const Vector2 &position) { _position = position; }
	Vector2* GetPtrPosition() const { return _ptrPosition; }
	void SetPtrPosition(Vector2 *ptrPosition) { _ptrPosition = ptrPosition; }
	float GetFlag() const { return _flag; }
	void SetFlag(const bool &flag) { _flag = flag; }
	void SetStr(const string &str) { _str = str; }
	string GetStr() const { return _str; }
	void PushToArray(float value) { _array.push_back(value); }
	float GetArray(unsigned i) { return _array[i]; }
	unsigned ArraySize() { return _array.size(); }

	// hard part
	void PushChild(CFoo* child) { _Children.push_back(child); }
	CFoo* GetElement(unsigned i) { return _Children[i]; }
	unsigned ChildrenCount() { return _Children.size(); }

private:
	bool _flag;
	string _str;
	vector<float> _array;
	Vector2 _position;
	Vector2 *_ptrPosition;
	vector<Vector2> _ArrPosition;
	vector<CFoo*> _Children;
};

class CBarDerived : public CFoo
{
	D2D_INJECT_TYPE_INFO(CBarDerived)

public:
	int GetH() const { return _h; }
	void SetH(const int &h) { _h = h; }
	CBarDerived* GetSingleBarPtr() const { return _singleBarPtr; }
	void SetSingleBarPtr( CBarDerived* rhs ) { _singleBarPtr = rhs; }

private:
	int _h;
	CBarDerived* _singleBarPtr;
};

template<typename T>
class CTFoo
{
};

class CAFoo
{
public:
	virtual void foo() = 0;
};

template<typename T>
class CTAFoo
{
public:
	virtual void foo() = 0;
};
