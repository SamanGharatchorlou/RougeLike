#pragma once

#include "Shape.h"

class Circle : public Shape
{
public:
	Circle() { }
	Circle(VectorF center, float radius, int pointCount);

	void setPosition(VectorF position) override;

private:
	VectorF mCenter;
};