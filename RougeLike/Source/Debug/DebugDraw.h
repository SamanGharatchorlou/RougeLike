#pragma once

#if _DEBUG
#define PRINT_FRAMERATE 0
#define DRAW_ENEMY_RECT 0
#define DRAW_PLAYER_RECTS 0
#define DRAW_BINARY_MAP 0
#define DRAW_AI_PATH 1
#else
#define PRINT_FRAMERATE 0
#define DRAW_ENEMY_RECT 0
#define DRAW_PLAYER_RECTS 0
#define DRAW_BINARY_MAP 0
#define DRAW_AI_PATH 0
#endif

struct RenderColour
{
	enum Colour
	{
		None,
		Red,
		Blue,
		Green,
		Yellow
	};

	RenderColour() { }

	RenderColour(Colour colour) 
	{
		a = 255;

		switch (colour)
		{
		case RenderColour::None:
			a = 0;
			break;
		case RenderColour::Red:
			r = 255; g = 0; b = 0;
			break;
		case RenderColour::Blue:
			r = 0; g = 0; b = 255;
			break;
		case RenderColour::Green:
			r = 0; g = 255; b = 0;
			break;
		case RenderColour::Yellow:
			r = 255; g = 255; b = 0;
		default:
			break;
		}
	}

	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 255;
};


void debugDrawLine(VectorF pointA, VectorF pointB, RenderColour colour);
void debugDrawRect(RectF rect, RenderColour colour);
void debugDrawRects(std::vector<RectF> rects, RenderColour colour);