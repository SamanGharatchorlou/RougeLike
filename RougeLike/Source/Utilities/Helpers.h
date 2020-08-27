#pragma once

constexpr float pi = 3.14f;


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

template<typename T, typename K>
float distance(Vector2D<T> pointA, Vector2D<K> pointB)
{
	return (float)sqrt((pointA.x - pointB.x) * (pointA.x - pointB.x) +
		(pointA.y - pointB.y) * (pointA.y - pointB.y));
}

template<typename T>
float magnitudeSqrd(Vector2D<T> vector)
{
	return (float)((vector.x * vector.x) + (vector.y * vector.y));
}


template<typename T>
void merge(std::vector<T>& vectorA, const std::vector<T>& vectorB)
{
	vectorA.insert(vectorA.end(), vectorB.begin(), vectorB.end());
}


int randomNumberBetween(int min, int max);


float getRotation(VectorF direction);
VectorF rotateVector(VectorF vector, float dTheta);
VectorF rotateVector(VectorF vector, float dTheta, VectorF about);
VectorF rotateVector(VectorF vector, VectorF about, double sine, double cosine);

VectorF closestRectSide(VectorF point, RectF rect);

VectorF realiseSize(VectorF baseSize, float maxDimention);


enum class Colour
{
	White,
	Red,
	Blue,
	Green,
	Yellow,
	LightGrey
};

SDL_Colour SDLColour(Colour colour);


int toInt(const BasicString& string);
float toFloat(const BasicString& string);

SDL_Rect toSDLRect(const RectF& rect);