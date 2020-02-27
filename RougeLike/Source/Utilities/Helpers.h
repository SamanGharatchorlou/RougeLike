#pragma once

constexpr float pi = 3.14f;


std::vector<int> parseCSVtoInt(std::string csv);


template<typename T>
T clamp(const T value, const T min, const T max)
{
	return std::min(max, std::max(value, min));
}


template<typename T>
Vector2D<T> clamp(Vector2D<T> vector, const T min, const T max)
{
	vector.x = std::min(max, std::max(vector.x, min));
	vector.y = std::min(max, std::max(vector.y, min));
	return vector;
}


template<typename T>
T clamp(Vector2D<T> vector, const Vector2D<T> min, const Vector2D<T> max)
{
	vector.x = std::min(max.x, std::max(vector.x, min.x));
	vector.y = std::min(max.y, std::max(vector.y, min.y));
	return vector;
}


template<typename T>
T toRadians(const T degrees)
{
	return degrees * static_cast<T>(pi / 180.0f);
}


template<typename T, typename K>
float distanceSquared(Vector2D<T> pointA, Vector2D<K> pointB)
{
	return (float)((pointA.x - pointB.x) * (pointA.x - pointB.x) +
					(pointA.y - pointB.y) * (pointA.y - pointB.y));
}


template<typename T>
T randomNumberBetween(T min, T max)
{
	return (rand() % (max - min)) + min;
}


double getRotation(VectorF direction);
VectorF rotateVector(VectorF vec, double dTheta);

VectorF closestRectSide(VectorF point, RectF rect);
