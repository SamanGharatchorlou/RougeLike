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

	Floor = 1 << 0,
	Floor_1 = Floor << 1,
	Floor_2 = Floor << 2,

	Floor_ColumnBase = Floor << 3,

	Floor_Right = Floor << 4,
	Floor_Left = Floor << 5,
	Floor_Top = Floor << 6,
	Floor_Bottom = Floor << 7,

	Floor_Top_Right = Floor << 8,
	Floor_Top_Left = Floor << 9,
	Floor_Bottom_Right = Floor << 10,
	Floor_Bottom_Left = Floor << 11,

	Floor_Grating = Floor << 12,
	Floor_Grating_Top = Floor << 13,

	FLOORS = Floor << 14,

	Water = FLOORS << 1,
	Water_Middle = Water << 1,
	Water_Left = Water << 2,
	Water_Top = Water << 3,
	Water_Top_Left = Water << 4,

	WATERS = Water << 5,

	Wall = WATERS << 1,

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

	WALLS = Wall << 17,

	Column = WALLS << 1,
	Column_Lower = Column << 1,
	Column_Upper = Column << 2,
	Column_Top = Column << 3,

	COLUMNS = Column << 4
};


enum class DecorType : Uint32
{
	None,
	Column = 1,
	Water = 1 << 1,

	ANIMATION = 1 << 2,

	Torch = ANIMATION << 1,
	Torch_Handle = ANIMATION << 2,
	Torch_Bowl = ANIMATION << 3,

	TRAP = ANIMATION << 4,

	Spikes = TRAP << 1,
	Grating = TRAP << 2,

	TRIGGER = TRAP << 3,

	GratingTrigger = TRIGGER << 1,

	END = TRIGGER << 2
};


//using DecorMap = std::map<DecorType, StringMap>;
DecorType stringToDecorType(const BasicString& decorString);



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

inline RenderTile operator ~(RenderTile a)
{
	return static_cast<RenderTile>(~static_cast<Uint64>(a));
}

inline RenderTile operator +(RenderTile a, int b)
{
	return static_cast<RenderTile>(static_cast<Uint64>(a) + static_cast<Uint64>(b));
}


// -- Decoration Tile -- //
inline DecorType operator |(DecorType a, DecorType b)
{
	return static_cast<DecorType>(static_cast<Uint32>(a) | static_cast<Uint32>(b));
}

inline DecorType operator &(DecorType a, DecorType b)
{
	return static_cast<DecorType>(static_cast<Uint32>(a) & static_cast<Uint32>(b));
}

inline DecorType operator << (DecorType type, int number)
{
	return static_cast<DecorType>(static_cast<Uint32>(type) << number);
}

inline DecorType operator ~(DecorType a)
{
	return static_cast<DecorType>(~static_cast<Uint32>(a));
}