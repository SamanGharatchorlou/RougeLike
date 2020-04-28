#include "pch.h"
#include "Square.h"



Square::Square(VectorF center, float sideLength)
{
	float halfSideLength = sideLength / 2.0f;

	mPoints.push_back(VectorF(-halfSideLength, -halfSideLength));
	mPoints.push_back(VectorF(halfSideLength, -halfSideLength));
	mPoints.push_back(VectorF(halfSideLength, halfSideLength));
	mPoints.push_back(VectorF(-halfSideLength, halfSideLength));
}


Square::Square(PointList points)
{
	mPoints = points;
}