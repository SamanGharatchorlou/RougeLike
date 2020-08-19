#pragma once

constexpr Uint8 alphaMax = 255;
constexpr Uint8 alphaMin = 0;

struct RenderColour
{
	enum Colour
	{
		None,
		Red,
		Blue,
		Green,
		Yellow,
		LightGrey,
		Black,
		White
	};

	RenderColour() : r(0), g(0), b(0), a(alphaMax) { }
	RenderColour(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = alphaMax);
	RenderColour(Colour colour);

	void setOpacity(float opacity);

	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};