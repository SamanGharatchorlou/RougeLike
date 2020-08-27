#pragma once

//#include "Shape.h"
//
//class Circle : public Shape
//{
//public:
//	Circle() { }
//	Circle(VectorF center, float radius)
//	{
//		// define a circle to have 8 points... why not
//		int points = 8;
//		float arc = (float)(360 / points);
//
//		// pointing up in the (0, -1) direction
//		VectorF radiusLine(0.0f, -radius);
//
//		for (int i = 0; i < points; i++)
//		{
//			VectorF point = center + (rotateVector(radiusLine, arc * i));
//			mPoints.push_back(point);
//		}
//	}
//
//	void setPosition(VectorF position) override
//	{
//		for (int i = 0; i < mPoints.size(); i++)
//		{
//			mPoints[i] = mPoints[i] - mCenter + position;
//		}
//
//		mCenter = position;
//	}
//
//private:
//	VectorF mCenter;
//};