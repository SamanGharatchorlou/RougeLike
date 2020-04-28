#pragma once

#include "Shape.h"

class Circle : public Shape
{
public:
	Circle(VectorF center, float radius, int pointCount);
};