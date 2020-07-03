#pragma once

class BasicTile
{
public:
	enum Type : Uint64
	{
		None,

		Floor,
		Floor_ColumnBase = Floor << 1,

		Floor_Right = Floor << 2,
		Floor_Left = Floor << 3,
		Floor_Top = Floor << 4,
		Floor_Bottom = Floor << 5,

		Floor_Top_Right = Floor << 6,
		Floor_Top_Left = Floor << 7,
		Floor_Bottom_Right = Floor << 8,
		Floor_Bottom_Left = Floor << 9,


		Wall = Floor << 12,

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

		Column_Lower = Wall << 17,
		Column_Upper = Wall << 18,
		Column_Top = Wall << 19,


		// Water
		Water = Wall << 20,
		Water_Top =  Wall << 21,

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
	return static_cast<Uint64>(a) != static_cast<Uint64>(b);
}

inline bool operator ==(BasicTile::Type a, BasicTile::Type b)
{
	return static_cast<Uint64>(a) == static_cast<Uint64>(b);
}


inline MapTile::Type operator &(BasicTile::Type a, BasicTile::Type b)
{
	return static_cast<BasicTile::Type>(static_cast<Uint64>(a) & static_cast<Uint64>(b));
}

inline void operator &=(BasicTile::Type& a, BasicTile::Type b)
{
	a = static_cast<BasicTile::Type>(static_cast<Uint64>(a) & static_cast<Uint64>(b));
}

inline BasicTile::Type operator |(BasicTile::Type a, BasicTile::Type b)
{
	return static_cast<BasicTile::Type>(static_cast<Uint64>(a) | static_cast<Uint64>(b));
}

inline void operator |=(BasicTile::Type& a, BasicTile::Type b)
{
	a = static_cast<BasicTile::Type>(static_cast<Uint64>(a) | static_cast<Uint64>(b));
}



inline BasicTile::Type operator ^(BasicTile::Type a, BasicTile::Type b)
{
	return static_cast<BasicTile::Type>(static_cast<Uint64>(a) ^ static_cast<Uint64>(b));
}

inline void operator ^=(BasicTile::Type& a, BasicTile::Type b)
{
	a = static_cast<BasicTile::Type>(static_cast<Uint64>(a) ^ static_cast<Uint64>(b));
}

inline BasicTile::Type operator ~(BasicTile::Type a)
{
	return static_cast<BasicTile::Type>(~static_cast<Uint64>(a));
}

