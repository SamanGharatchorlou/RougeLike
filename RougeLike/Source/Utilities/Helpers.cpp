#include "pch.h"
#include "Helpers.h"


std::vector<int> parseCSVtoInt(std::string csv)
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



// (0,1) = 0 degrees
float getRotation(VectorF direction)
{
	return (std::atan2(direction.y, direction.x) * 180.0f / pi) + 90.0f;
}


// including min, not max
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

