#pragma once

#include "Formation.h"

class Point : public Formation
{
public:
	Point(VectorF position)
	{
		mPoints.push_back(position);
	}

	void setPosition(VectorF position)
	{
		mPoints.front() = VectorF(position);
	}
};

