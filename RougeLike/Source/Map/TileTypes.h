#pragma once


enum class CollisionTile : Uint32
{
	None,
	Floor,
	Wall = Floor << 1,
	Water = Floor << 2
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

	Water = Floor << 10,

	Water_Middle = Water << 1,
	Water_Left = Water << 2,
	Water_Top = Water << 3,
	Water_Top_Left = Water << 4,

	Wall = Water << 5,

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
};


enum class DecorTile : Uint32
{
	None,
	Column = 1,
	Water = Column << 1,
	Torch_Handle = Column << 2,
	Torch_Bowl = Column << 3,
	Spikes = Column << 4
};



// -- Collision Tile -- //
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

inline RenderTile operator ~(RenderTile a)
{
	return static_cast<RenderTile>(~static_cast<Uint64>(a));
}

//inline bool operator >(RenderTile a, RenderTile b)
//{
//	return static_cast<Uint64>(a) > static_cast<Uint64>(b);
//}


// -- Decoration Tile -- //
inline DecorTile operator |(DecorTile a, DecorTile b)
{
	return static_cast<DecorTile>(static_cast<Uint32>(a) | static_cast<Uint32>(b));
}

inline DecorTile operator &(DecorTile a, DecorTile b)
{
	return static_cast<DecorTile>(static_cast<Uint32>(a) & static_cast<Uint32>(b));
}