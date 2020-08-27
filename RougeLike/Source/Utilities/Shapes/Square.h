#pragma once
//
//#include "Shape.h"
//
//class Square : public Shape
//{
//public:
//	Square(VectorF center, float sideLength) 
//	{
//		build(center, sideLength);
//	}
//
//	Square(PointList points)
//	{
//		mPoints = points;
//	}
//
//	void setPosition(VectorF position) override
//	{
//		float sideLength = std::abs(mPoints[0].x - mPoints[1].x);
//		build(position, sideLength);
//	}
//
//
//private:
//	void build(VectorF position, float sideLength)
//	{
//		float halfSideLength = sideLength / 2.0f;
//
//		mPoints.push_back(VectorF(-halfSideLength, -halfSideLength));
//		mPoints.push_back(VectorF(halfSideLength, -halfSideLength));
//		mPoints.push_back(VectorF(halfSideLength, halfSideLength));
//		mPoints.push_back(VectorF(-halfSideLength, halfSideLength));
//	}
//};