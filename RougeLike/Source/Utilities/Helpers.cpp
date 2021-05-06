#include "pch.h"
#include "Helpers.h"

VectorF rotateVector(VectorF vector, float dTheta, VectorF about)
{
	double radians = toRadians(dTheta);
	double cosine = cos(radians);
	double sine = sin(radians);

	return rotateVector(vector, about, sine, cosine);
}

VectorF rotateVector(VectorF vector, VectorF about, double sine, double cosine)
{
	VectorF originPoint = vector - about;

	float x = (originPoint.x * cosine) - (originPoint.y * sine);
	float y = (originPoint.x * sine) + (originPoint.y * cosine);

	return VectorF(x, y) + about;
}


// (0,1) = 0 degrees
float getRotation(VectorF direction)
{
	return (std::atan2(direction.y, direction.x) * 180.0f / pi);
}


// including min, not max (min, max]
int randomNumberBetween(int min, int max)
{
	return (rand() % (max - min)) + min;
}



VectorF closestRectSide(VectorF point, RectF rect)
{
	VectorF rectSides[] = {
		rect.TopCenter(),
		rect.RightCenter(),
		rect.BotCenter(),
		rect.LeftCenter()
	};

	int nearestSide = 0;
	float leastDistance = distanceSquared(point, rectSides[0]);

	for (int i = 1; i < 4; i++)
	{
		float distance = distanceSquared(point, rectSides[i]);

		if (distance < leastDistance)
		{
			nearestSide = i;
			leastDistance = distance;
		}
	}

	return rectSides[nearestSide];
}


VectorF realiseSize(VectorF baseSize, float maxDimention)
{
	VectorF ratio = baseSize / maxDimention;
	float maxRatio = std::max(ratio.x, ratio.y);
	return baseSize / maxRatio;
}



SDL_Colour SDLColour(Colour colour)
{
	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;

	switch (colour)
	{
	case Colour::White:
		r = 255; g = 255; b = 255;
		break;
	case Colour::Red:
		r = 255; g = 0; b = 0;
		break;
	case Colour::Blue:
		r = 0; g = 0; b = 255;
		break;
	case Colour::Green:
		r = 0; g = 255; b = 0;
		break;
	case Colour::Yellow:
		r = 255; g = 255; b = 0;
		break;
	case Colour::LightGrey:
		r = 201; g = 201; b = 201;
		break;
	default:
		DebugPrint(Warning, "Colour enum %d not recognised\n", (int)colour);
		break;
	}

	return SDL_Color{ r, g, b };
}



int toInt(const BasicString& string)
{
	return atoi(string.c_str());
}

float toFloat(const BasicString& string)
{
	return (float)atof(string.c_str());
}

SDL_Rect toSDLRect(const RectF& rect)
{
	return SDL_Rect {	static_cast<int>(rect.x1 + 0.5f),
						static_cast<int>(rect.y1 + 0.5f),
						static_cast<int>(rect.Width() + 0.5f),
						static_cast<int>(rect.Height() + 0.5f) };
}