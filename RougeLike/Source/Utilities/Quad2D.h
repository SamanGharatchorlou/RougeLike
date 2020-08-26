#pragma once

#include "Vector2D.h"
#include "Rect.h"

template<class T>
class Quad2D
{
public:
	using Point = Vector2D<T>;

public:
	Quad2D(Point point1, Point point2, Point point3, Point point4)
	{
		points.push_back(point1);
		points.push_back(point2);
		points.push_back(point3);
		points.push_back(point4);
	}

	Quad2D(const Rect<T>& rect)
	{
		points.reserve(sides());
		points.push_back(rect.TopLeft());
		points.push_back(rect.TopRight());
		points.push_back(rect.BotRight());
		points.push_back(rect.BotLeft());
	}

	Point& operator [] (int index) { return points[index]; }
	Point at(int index) const { return points.at(index); }

	void rotate(double degrees, Point aboutpoint);

	int sides() const { return 4; }

	T xMin() const;
	T xMax() const;
	T yMin() const;
	T yMax() const;

	Point normal0() const;
	Point normal1() const;


private:
	std::vector<Point> points;
};



template<class T>
Vector2D<T> Quad2D<T>::normal0() const
{
	float dx = points[1].x - points[0].x;
	float dy = points[1].y - points[0].y;

	return VectorF(dy, -dx).normalise();
}


template<class T>
Vector2D<T> Quad2D<T>::normal1() const
{
	float dx = points[2].x - points[1].x;
	float dy = points[2].y - points[1].y;

	return VectorF(dy, -dx).normalise();
}



template<class T>
void Quad2D<T>::rotate(double degrees, Point aboutpoint)
{
	double radians = toRadians(degrees);
	double cosine = cos(radians);
	double sine = sin(radians);

	for (int i = 0; i < sides(); i++)
	{
		points[i] = rotateVector(points[i], aboutpoint, sine, cosine);;
	}
}


template<class T>
T Quad2D<T>::xMin() const
{
	T minX = points[0].x;
	for (int i = 0; i < sides(); i++)
	{
		if (minX > points[i].x)
			minX = points[i].x;
	}

	return minX;
}

template<class T>
T Quad2D<T>::xMax() const
{
	T maxX = points[0].x;
	for (int i = 0; i < sides(); i++)
	{
		if (maxX < points[i].x)
			maxX = points[i].x;
	}

	return maxX;
}


template<class T>
T Quad2D<T>::yMin() const
{
	T minY = points[0].y;
	for (int i = 0; i < sides(); i++)
	{
		if (minY > points[i].y)
			minY = points[i].y;
	}

	return minY;
}

template<class T>
T Quad2D<T>::yMax() const
{
	T maxY = points[0].y;
	for (int i = 0; i < sides(); i++)
	{
		if (maxY < points[i].y)
			maxY = points[i].y;
	}

	return maxY;
}