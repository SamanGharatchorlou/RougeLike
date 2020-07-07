#pragma once


class Texture;

enum class CollisionTile : Uint32
{
	None,
	Floor,
	Wall,
	Water,
};

enum class RenderTile : Uint64
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

	Water = Column_Top << 20,

	Water_Middle = Water << 1,
	Water_Top = Water << 2,
};


enum class AnimationTile : Uint32
{
	None,
	Torch
};



class BasicTile
{
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
	PathTile() : mCollisionType(CollisionTile::None) { }
	PathTile(CollisionTile type, RectF rect) : mCollisionType(type), BasicTile(rect) { }

	// Collision type
	const CollisionTile collisionType() const { return mCollisionType; }
	virtual bool is(CollisionTile type) const;

	virtual void set(CollisionTile type) { mCollisionType = type; }
	void add(CollisionTile type);
	void remove(CollisionTile type);


protected:
	CollisionTile mCollisionType;
};


// Add rendering info and texture
class MapTile : public PathTile
{
public:
	MapTile() : mRenderType(RenderTile::None), mAnimationType(AnimationTile::None), mTexture(nullptr) { }

	void setTexture(Texture* texture) { mTexture = texture; }

	// Render type
	const RenderTile renderType() const { return mRenderType; }
	bool has(RenderTile type) const;
	bool is(RenderTile type) const;
	void set(RenderTile type) { mRenderType = type; }

	void add(RenderTile type);
	void remove(RenderTile type);

	// Collision type
	void set(CollisionTile type) { PathTile::set(type); }
	bool is(CollisionTile type) const { return PathTile::is(type); }

	// Animation type
	void set(AnimationTile type) { mAnimationType = type; }
	bool is(AnimationTile type) const;

	void render(RectF rect);

private:
	Texture* mTexture;

	RenderTile mRenderType;
	AnimationTile mAnimationType;
};


// -- Collision Tile -- //
inline bool operator !=(CollisionTile a, CollisionTile b)
{
	return static_cast<Uint32>(a) != static_cast<Uint32>(b);
}

inline CollisionTile operator &(CollisionTile a, CollisionTile b)
{
	return static_cast<CollisionTile>(static_cast<Uint32>(a) & static_cast<Uint32>(b));
}

inline CollisionTile operator |(CollisionTile a, CollisionTile b)
{
	return static_cast<CollisionTile>(static_cast<Uint32>(a) | static_cast<Uint32>(b));
}

inline CollisionTile operator ~(CollisionTile a)
{
	return static_cast<CollisionTile>(~static_cast<Uint32>(a));
}

// -- Render Tile -- //
inline bool operator !=(RenderTile a, RenderTile b)
{
	return static_cast<Uint64>(a) != static_cast<Uint64>(b);
}

inline bool operator ==(RenderTile a, RenderTile b)
{
	return static_cast<Uint64>(a) == static_cast<Uint64>(b);
}


inline RenderTile operator &(RenderTile a, RenderTile b)
{
	return static_cast<RenderTile>(static_cast<Uint64>(a) & static_cast<Uint64>(b));
}

inline RenderTile operator |(RenderTile a, RenderTile b)
{
	return static_cast<RenderTile>(static_cast<Uint64>(a) | static_cast<Uint64>(b));
}


inline RenderTile operator ^(RenderTile a, RenderTile b)
{
	return static_cast<RenderTile>(static_cast<Uint64>(a) ^ static_cast<Uint64>(b));
}

inline void operator ^=(RenderTile& a, RenderTile b)
{
	a = static_cast<RenderTile>(static_cast<Uint64>(a) ^ static_cast<Uint64>(b));
}

inline RenderTile operator ~(RenderTile a)
{
	return static_cast<RenderTile>(~static_cast<Uint64>(a));
}

inline bool operator >(RenderTile a, RenderTile b)
{
	return static_cast<Uint64>(a) > static_cast<Uint64>(b);
}