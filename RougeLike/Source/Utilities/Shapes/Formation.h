#pragma once

class AIPathMap;

using PointList = std::vector<VectorF>;


class Formation
{
public:
	virtual ~Formation() { }

	const PointList& points() const { return mPoints; }

	virtual void setPosition(VectorF position) = 0;

	bool isValid(const AIPathMap* map) const;

protected:
	PointList mPoints;
};

