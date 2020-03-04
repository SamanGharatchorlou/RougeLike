#pragma once

#define PRINT_FRAMERATE 0
#define DRAW_PLAYER_RECT 0
#define DRAW_WEAPON_RECTS 0
#define DRAW_BINARY_MAP 0
#define DRAW_ENEMY_RECT 0

struct GameData;

struct RenderColour
{
	enum Colour
	{
		NONE,
		RED,
		BLUE,
		GREEN,
	};

	RenderColour() { }
	RenderColour(Colour colour) 
	{
		switch (colour)
		{
		case RenderColour::NONE:
			a = 0;
			break;
		case RenderColour::RED:
			r = 255; g = 0; b = 0; a = 255;
			break;
		case RenderColour::BLUE:
			r = 0; g = 0; b = 255; a = 255;
			break;
		case RenderColour::GREEN:
			r = 0; g = 255; b = 0; a = 255;
			break;
		default:
			break;
		}
	}

	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 255;
};

void debugDrawRect(GameData* gameData, RectF rect, RenderColour colour);
void debugDrawRects(GameData* gameData, std::vector<RectF> rects, RenderColour colour);