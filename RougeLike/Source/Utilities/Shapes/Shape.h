#pragma once

using PointList = std::vector<VectorF>;

class Shape
{
public:
	PointList points() const { return mPoints; }

protected:
	PointList mPoints;
};