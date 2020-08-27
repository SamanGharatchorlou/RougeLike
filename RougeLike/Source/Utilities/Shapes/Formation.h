#pragma once

#include "Utilities/Vector2D.h"

using PointList = std::vector<VectorF>;

class Formation
{
public:
	virtual ~Formation() { }

	const PointList& points() const { return mPoints; }

	virtual void setPosition(VectorF position) = 0;
	//{
	//	DebugPrint(Warning, "I'm an ambiguous formation so I dont know how to move my points");
	//}

protected:
	PointList mPoints;
};

