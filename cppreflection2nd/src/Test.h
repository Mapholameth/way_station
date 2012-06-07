#pragma once

class Vector2
{
public:
	void SetX(const float &x) { _x = x; }
	void SetY(const float &y) { _y = y; }	
	void SetFlag(const bool &flag) { _flag = flag; }
	float GetX() const { return _x; }
	float GetY() const { return _y; }
	float GetFlag() const { return _flag; }

private:
	float _x;
	float _y;
	bool _flag;
};

class CFoo
{
public:
	Vector2 GetPosition() const { return _position; }
	void SetPosition(const Vector2 &position) { _position = position; }

private:
	Vector2 _position;
};

/*
class CFoo
{
};

class CBar : public CFoo
{
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
};*/
