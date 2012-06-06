#pragma once

class Vector2
{
public:
	float x;
	float y;
};

class CFoo
{
public:
	Vector2 position() const { return _position; }
	void setPosition(const Vector2 &position) { _position = position; }

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