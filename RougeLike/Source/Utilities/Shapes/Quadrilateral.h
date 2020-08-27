#pragma once

#include "Formation.h"

// Can have more than four points, but still keeps its shape e.g.
// * * * *
// * * * *
class Quadrilateral : public Formation
{
public:
	Quadrilateral(VectorF topLeft, int xPoints, int yPoints, float pointSpacing)
	{
		for (int y = 0; y < yPoints; y++)
		{
			for (int x = 0; x < xPoints; x++)
			{
				VectorF position = topLeft + (VectorF(x, y) * pointSpacing);
				mPoints.push_back(position);
			}
		}
	}

	void setPosition(VectorF position)
	{
		VectorF movement = position - mPoints[0];

		for (int i = 0; i < mPoints.size(); i++)
		{
			mPoints[i] += movement;
		}

	}


private:
	//void build(VectorF topLeft, int xPoints, int yPoints, float pointSpacing)
	//{
	//	for (int y = 0; y < yPoints; y++)
	//	{
	//		for (int x = 0; x < xPoints; x++)
	//		{
	//			VectorF position = topLeft + (VectorF(x, y) * pointSpacing);
	//			mPoints.push_back(position);
	//		}
	//	}
	//}
};