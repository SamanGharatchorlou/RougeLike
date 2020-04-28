#pragma once

#include "Shape.h"

class Square : public Shape
{
public:
	Square(VectorF center, float sideLength);
	Square(PointList points);
};