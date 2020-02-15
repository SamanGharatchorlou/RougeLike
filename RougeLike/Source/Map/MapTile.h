#pragma once



class MapTile
{
public:
	enum Type
	{
		None,
		Floor,
		Wall,

		Left = Wall << 1,
		Right = Wall << 2,

		Top = Wall << 3,
		Bot = Wall << 4,
	};

	struct EdgeInfo
	{
		Type data[3][3] = { { Wall,Wall,Wall }, { Wall,Wall,Wall }, { Wall,Wall,Wall } };
		bool hasEdge = false;
		bool draw = true;
	};


public:
	MapTile() : mRenderType(Wall), mCollisionType(Wall), mRect(-1) { printf("creating Tile\n"); }
	MapTile(Type type) : mRenderType(type), mCollisionType(type), mRect(-1) { }
	MapTile(RectF rect) : mRect(rect), mRenderType(Wall), mCollisionType(Wall) { }

	RectF& rect() { return mRect; }
	void setRect(RectF rect) { mRect = rect; }

	void setType(Type type) { mRenderType = type; mCollisionType = type; }

	Type renderType() { return mRenderType; }
	bool hasRenderType(Type type);
	void setRenderType(Type type) { mRenderType = type; }
	void addRenderType(Type type);
	void removeRenderType(Type type);

	Type collisionType() { return mCollisionType; }
	bool hasCollisionType(Type type);
	void setCollisionType(Type type) { mCollisionType = type; }
	void addCollisionType(Type type);
	void removeCollisionType(Type type);

	EdgeInfo& info() { return mInfo; }
	void setInfo(EdgeInfo info) { mInfo = info; }

#if _DEBUG
	Vector2D<int> index;
#endif

private:
	RectF mRect;

	Type mRenderType;
	Type mCollisionType;

	EdgeInfo mInfo;
};



inline bool operator !=(MapTile::Type a, MapTile::Type b)
{
	return static_cast<int>(a) != static_cast<int>(b);
}

inline bool operator ==(MapTile::Type a, MapTile::Type b)
{
	return static_cast<int>(a) == static_cast<int>(b);
}


inline MapTile::Type operator &(MapTile::Type a, MapTile::Type b)
{
	return static_cast<MapTile::Type>(static_cast<int>(a) & static_cast<int>(b));
}

inline void operator &=(MapTile::Type& a, MapTile::Type b)
{
	a = static_cast<MapTile::Type>(static_cast<int>(a) & static_cast<int>(b));
}

inline MapTile::Type operator |(MapTile::Type a, MapTile::Type b)
{
	return static_cast<MapTile::Type>(static_cast<int>(a) | static_cast<int>(b));
}

inline void operator |=(MapTile::Type& a, MapTile::Type b)
{
	a = static_cast<MapTile::Type>(static_cast<int>(a) | static_cast<int>(b));
}



inline MapTile::Type operator ^(MapTile::Type a, MapTile::Type b)
{
	return static_cast<MapTile::Type>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline void operator ^=(MapTile::Type& a, MapTile::Type b)
{
	a = static_cast<MapTile::Type>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline MapTile::Type operator ~(MapTile::Type a)
{
	return static_cast<MapTile::Type>(~static_cast<int>(a));
}

