#include "pch.h"
#include "Helpers.h"


std::vector<int> parseCSVtoInt(const std::string& csv)
{
	std::vector<int> vect;

	std::stringstream ss(csv);

	for (int i = 0; ss >> i;)
	{
		vect.push_back(i);

		if (ss.peek() == ',' || ss.peek() == ' ')
			ss.ignore();
	}

	return vect;
}


// x' = x cos - y sin
// y' = x sin + y cos
VectorF rotateVector(VectorF vector, float dTheta)
{
	VectorF outVector;
	float radians = toRadians(dTheta);

	outVector.x = vector.x * cos(radians) - vector.y * sin(radians);
	outVector.y = (vector.x * sin(radians) + vector.y * cos(radians));

	return outVector;
}


// NOTE: untested
VectorF rotateVector(VectorF vector, float dTheta, VectorF about)
{
	VectorF outVector;
	float radians = toRadians(dTheta);

	outVector.x =  ((vector.x - about.x) * cos(radians)) - ((about.y - vector.y) * sin(radians)) + about.x;
	outVector.y = -((vector.x - about.x) * sin(radians)) + ((about.y - vector.y) * cos(radians)) + about.y;

	return outVector;
}


// (0,1) = 0 degrees
float getRotation(VectorF direction)
{
	return (std::atan2(direction.y, direction.x) * 180.0f / pi) + 90.0f;
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