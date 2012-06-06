#pragma once

class Vector2
{
public:
	void SetX(const float &x) { _x = x; }
	void SetY(const float &y) { _y = y; }

private:
	float _x;
	float _y;
};

class CFoo
{
public:
	Vector2 position() const { return _position; }
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
