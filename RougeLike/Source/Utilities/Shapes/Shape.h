#pragma once

using PointList = std::vector<VectorF>;

class Shape
{
public:
	PointList points() const { return mPoints; }

	virtual void setPosition(VectorF position) { }

protected:
	PointList mPoints;
};