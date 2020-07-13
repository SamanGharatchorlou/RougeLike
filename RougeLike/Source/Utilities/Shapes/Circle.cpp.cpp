#include "pch.h"
#include "Circle.h"

Circle::Circle(VectorF center, float radius, int pointCount)
{
	ASSERT(Warning, pointCount > 5, "A circle needs at least 6 points to be a circle, this one has %d\n", pointCount);

	float arc = (float)(360 / pointCount);

	// pointing up in the (0, -1) direction
	VectorF radiusLine(0.0f, -radius);
	float arcRotation = 0.0f;

	while (arcRotation < 360)
	{
		VectorF point = center + (rotateVector(radiusLine, arcRotation));
		mPoints.push_back(point);
		arcRotation += arc;
	}
}