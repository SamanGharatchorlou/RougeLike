#pragma once


class BasicTile
{
public:
	enum Type : int32_t
	{
		None,
		Floor,
		Wall,

		// Sides
		Left = Wall << 1,
		Right = Wall << 2,

		// Top / Bottom
		Bottom_Lower = Wall << 3,
		Bottom_Upper = Wall << 4,

		Top_Lower = Wall << 5,
		Top_Upper = Wall << 6,

		// Top/Bottom ends
		Bottom = Wall << 7,
		Top = Wall << 8,

		// Corners
		Top_Right = Wall << 9,
		Top_Left = Wall << 10,
		Bottom_Right = Wall << 11,
		Bottom_Left = Wall << 12,

		// Point corners
		Point_Bottom_Right = Wall << 13,
		Point_Bottom_Left = Wall << 14,
		Point_Top_Right = Wall << 15,
		Point_Top_Left = Wall << 16,
	};

public:
	BasicTile() : mRect(-1) { }
	BasicTile(RectF rect) : mRect(rect) { }

	RectF rect() const { return mRect; }
	void setRect(RectF rect) { mRect = rect; }

protected:
	RectF mRect;
};



// Add collisions
class PathTile : public BasicTile
{
public:
	PathTile() : mCollisionType(Wall) { }
	PathTile(Type type) : mCollisionType(type) { }
	PathTile(RectF rect) : BasicTile(rect), mCollisionType(Wall) { }
	PathTile(Type type, RectF rect) : mCollisionType(type), BasicTile(rect) { }

	// Collision type
	const Type collisionType() const { return mCollisionType; }
	bool hasCollisionType(Type type) const;

	void setCollisionType(Type type) { mCollisionType = type; }
	void addCollisionType(Type type);
	void removeCollisionType(Type type);


protected:
	Type mCollisionType;
};


class Texture;

// Add rendering info and texture
class MapTile : public PathTile
{
public:
	MapTile() : mRenderType(Wall), mTexture(nullptr) { }
	MapTile(Type type) : mRenderType(type), PathTile(type), mTexture(nullptr) { }
	MapTile(RectF rect) : PathTile(rect), mRenderType(Wall), mTexture(nullptr) { }

	void setTexture(Texture* texture) { mTexture = texture; }

	// Render type
	const Type renderType() const { return mRenderType; }
	bool hasRenderType(Type type) const;
	bool isRenderType(Type type) const;

	void setRenderType(Type type) { mRenderType = type; }
	void addRenderType(Type type);
	void removeRenderType(Type type);

	void render(RectF rect);

#if _DEBUG
	Vector2D<int> index;
#endif

private:
	Type mRenderType;
	Texture* mTexture;
};



inline bool operator !=(BasicTile::Type a, BasicTile::Type b)
{
	return static_cast<int>(a) != static_cast<int>(b);
}

inline bool operator ==(BasicTile::Type a, BasicTile::Type b)
{
	return static_cast<int>(a) == static_cast<int>(b);
}


inline MapTile::Type operator &(BasicTile::Type a, BasicTile::Type b)
{
	return static_cast<BasicTile::Type>(static_cast<int>(a) & static_cast<int>(b));
}

inline void operator &=(BasicTile::Type& a, BasicTile::Type b)
{
	a = static_cast<BasicTile::Type>(static_cast<int>(a) & static_cast<int>(b));
}

inline BasicTile::Type operator |(BasicTile::Type a, BasicTile::Type b)
{
	return static_cast<BasicTile::Type>(static_cast<int>(a) | static_cast<int>(b));
}

inline void operator |=(BasicTile::Type& a, BasicTile::Type b)
{
	a = static_cast<BasicTile::Type>(static_cast<int>(a) | static_cast<int>(b));
}



inline BasicTile::Type operator ^(BasicTile::Type a, BasicTile::Type b)
{
	return static_cast<BasicTile::Type>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline void operator ^=(BasicTile::Type& a, BasicTile::Type b)
{
	a = static_cast<BasicTile::Type>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline BasicTile::Type operator ~(BasicTile::Type a)
{
	return static_cast<BasicTile::Type>(~static_cast<int>(a));
}

