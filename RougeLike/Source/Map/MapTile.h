#pragma once


class BasicTile
{
public:
	enum Type
	{
		None,
		Floor,
		Wall,

		Left = Wall << 1,
		Right = Wall << 2,

		Bottom_Lower = Wall << 3,
		Bottom_Upper = Wall << 4,


		Top_Lower = Wall << 5,
		Top_Upper = Wall << 6,

		Top_Right = Wall << 7,
		Top_Left = Wall << 8,
		Bot_Right = Wall << 9,
		Bot_Left = Wall << 10
	};

public:
	BasicTile() : mRect(-1) { }
	BasicTile(RectF rect) : mRect(rect) { }

	//RectF& rectRef() { return mRect; }
	const RectF* rectPtr() const { return &mRect; }
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



// Add rendering and edge info
class MapTile : public PathTile
{
public:
	struct EdgeInfo
	{
		Type data[3][3] = { { Wall,Wall,Wall }, { Wall,Wall,Wall }, { Wall,Wall,Wall } };
		bool hasEdge = false;
		bool draw = true;
	};


public:
	MapTile() : mRenderType(Wall) { }
	MapTile(Type type) : mRenderType(type), PathTile(type) { }
	MapTile(RectF rect) : PathTile(rect), mRenderType(Wall) { }

	void setType(Type type) { mRenderType = type; mCollisionType = type; }

	// Render type
	const Type renderType() const { return mRenderType; }
	bool hasRenderType(Type type) const;
	bool isRenderType(Type type) const;

	void setRenderType(Type type) { mRenderType = type; }
	void addRenderType(Type type);
	void removeRenderType(Type type);


	const EdgeInfo* info() const { return &mInfo; }
	void setInfo(EdgeInfo info) { mInfo = info; }

#if _DEBUG
	Vector2D<int> index;
#endif

private:
	Type mRenderType;

	EdgeInfo mInfo;
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

