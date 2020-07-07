//#pragma once
//
//enum class TileLogicType
//{
//	None,
//	Wall,
//	Render,
//	Animation
//};
//
//
//class TileType
//{
//public:
//	enum Type : Uint64
//	{
//		None,
//
//		Floor,
//		Floor_ColumnBase = Floor << 1,
//
//		Floor_Right = Floor << 2,
//		Floor_Left = Floor << 3,
//		Floor_Top = Floor << 4,
//		Floor_Bottom = Floor << 5,
//
//		Floor_Top_Right = Floor << 6,
//		Floor_Top_Left = Floor << 7,
//		Floor_Bottom_Right = Floor << 8,
//		Floor_Bottom_Left = Floor << 9,
//
//
//		Wall = Floor << 12,
//
//		// Sides
//		Left = Wall << 1,
//		Right = Wall << 2,
//
//		// Top / Bottom
//		Bottom_Lower = Wall << 3,
//		Bottom_Upper = Wall << 4,
//
//		Top_Lower = Wall << 5,
//		Top_Upper = Wall << 6,
//
//		// Top/Bottom ends
//		Bottom = Wall << 7,
//		Top = Wall << 8,
//
//		// Corners
//		Top_Right = Wall << 9,
//		Top_Left = Wall << 10,
//		Bottom_Right = Wall << 11,
//		Bottom_Left = Wall << 12,
//
//		// Point corners
//		Point_Bottom_Right = Wall << 13,
//		Point_Bottom_Left = Wall << 14,
//		Point_Top_Right = Wall << 15,
//		Point_Top_Left = Wall << 16,
//
//		Column_Lower = Wall << 17,
//		Column_Upper = Wall << 18,
//		Column_Top = Wall << 19,
//
//		Tourch = Wall << 20,
//
//
//		// Water
//		Water = Wall << 21,
//		Water_Top = Wall << 22,
//
//	};
//
//
//	TileType() : logicType(TileLogicType::None), renderType(RenderType::None) { };
//
//	TileLogicType logicType;
//
//	RenderType renderType;
//
//
//	
//};