#pragma once

struct Color
{
	float r, g, b, a;

	static const Color Red();
	static const Color Green();
	static const Color Blue();
	static const Color Yellow();
	static const Color Magenta();
	static const Color Cyan();
	static const Color White();
	static const Color Gray();
	static const Color Black();
	static const Color Clear();

	// default constructor sets the color to white
	Color();
	// copy a predefined color (either static or user declared)
	Color(const Color& copy);
	// custom color
	Color(float r, float b, float g, float a = 1);

	Color& operator=(const Color& a);
	Color operator+(Color a) const;
	Color operator+=(Color a);
	Color operator-(Color a) const;
	Color operator-=(Color a);
	Color operator*(Color a) const;
	Color operator*(float a) const;
	Color operator*=(Color a);
	Color operator*=(float a);

	static Color Lerp(Color a, Color b, float t);
};
