#include "Color.h"

const Color Color::Red()
{
	return Color{ 1, 0, 0, 1 };
}

const Color Color::Green()
{
	return Color{ 0, 1, 0, 1 };
}

const Color Color::Blue()
{
	return Color{ 0, 0, 1, 1 };
}

const Color Color::Yellow()
{
	return Color{ 1, 0.92f, 0.016f, 1 };
}

const Color Color::Magenta()
{
	return Color{ 1, 0, 1, 1 };
}

const Color Color::Cyan()
{
	return Color{ 0, 1, 1, 1 };
}

const Color Color::White()
{
	return Color{ 1, 1, 1, 1 };
}

const Color Color::Gray()
{
	return Color{ 0.5f, 0.5f, 0.5f, 1 };
}

const Color Color::Black()
{
	return Color{ 0, 0, 0, 1 };
}

const Color Color::Clear()
{
	return Color{ 0, 0, 0, 0 };
}

Color::Color() : r{ 1.0f }, g{ 1.0f }, b{ 1.0f }, a{ 1.0f }
{
}

Color::Color(const Color& copy) : r{ copy.r }, g{ copy.g }, b{ copy.b }, a{ copy.a }
{
}

Color::Color(float c0, float c1, float c2, float c3) : r{ c0 }, g{ c1 }, b{ c2 }, a{ c3 }
{
}

Color& Color::operator=(const Color& c)
{
	this->r = c.r;
	this->g = c.g;
	this->b = c.b;
	this->a = c.a;
	return *this;
}

Color Color::operator+(const Color c) const
{
	return Color(r + c.r, g + c.g, b + c.b, a + c.a);
}

Color Color::operator+=(const Color c)
{
	this->r += c.r;
	this->g += c.g;
	this->b += c.b;
	this->a += c.a;
	return *this;
}

Color Color::operator-(const Color c) const
{
	return Color(r - c.r, g - c.g, b - c.b, a - c.a);
}

Color Color::operator-=(const Color c)
{
	this->r -= c.r;
	this->g -= c.g;
	this->b -= c.b;
	this->a -= c.a;
	return *this;
}

Color Color::operator*(const Color f) const
{
	return Color(r * f.r, g * f.g, b * f.b, a * f.a);
}

Color Color::operator*(const float f) const
{
	return Color(r * f, g * f, b * f, a * f);
}

Color Color::operator*=(const Color f)
{
	this->r *= f.r;
	this->g *= f.g;
	this->b *= f.b;
	this->a *= f.a;
	return *this;
}

Color Color::operator*=(const float f)
{
	this->r *= f;
	this->g *= f;
	this->b *= f;
	this->a *= f;
	return *this;
}

Color Color::Lerp(Color a, Color b, float t)
{
	return a + (b - a) * t;
}