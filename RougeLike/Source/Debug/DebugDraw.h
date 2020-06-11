#pragma once

#if _DEBUG

// Framerate
#define FRAMERATE_CAP 0 // Use the FPS cap you want
#define PRINT_FRAMERATE_EVERY 0 // Print at the rate you specify in milliseconds i.e. 500 = print every 0.5 seconds

// Player
#define DRAW_PLAYER_RECTS 0

// Collectables
#define DRAW_COLLECTABLE_RECT 0

// Map
#define DRAW_BINARY_MAP 0
#define MARK_SURFACE_TYPES 0

// Enemy / AI
#define DRAW_AI_PATH 0
#define DRAW_AI_PATH_COSTMAP 0
#define DRAW_ENEMY_RECTS 0
#define DRAW_ENEMY_TARGET_PATH 0
#define LIMIT_ENEMY_SPAWNS 0 // Set to the max number of enemies you want to spawn + 1 i.e. set to 1 to prevent any spawning

// UI
#define DRAW_UI_RECTS 0
#define UI_EDITOR 0
#define DEBUG_CURSOR 1

#else
#define FRAMERATE_CAP 0
#define PRINT_FRAMERATE_EVERY 500
#define DRAW_ENEMY_RECT 0
#define DRAW_PLAYER_RECTS 0
#define DRAW_BINARY_MAP 0
#define MARK_SURFACE_TYPES 0
#define DRAW_AI_PATH 0
#define DRAW_ENEMY_TARGET_PATH 0
#define LIMIT_ENEMY_SPAWNS 0
#define DRAW_COLLECTABLE_RECT 0
#define DRAW_UI_RECTS 0
#define UI_EDITOR 0
#define DEBUG_CURSOR 0
#endif

struct RenderColour
{
	enum Colour
	{
		None,
		Red,
		Blue,
		Green,
		Yellow,
		LightGrey
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
			break;
		case RenderColour::LightGrey:
			r = 201; g = 201; b = 201;
			break;
		default:
			break;
		}
	}

	void setOpacity(float opacity)
	{
		opacity = clamp(opacity, 0.0f, 1.0f);
		a = 255 * opacity;
	}

	Uint8 r = 0;
	Uint8 g = 0;
	Uint8 b = 0;
	Uint8 a = 255;
};


void debugDrawLine(VectorF pointA, VectorF pointB, RenderColour colour);
void debugDrawRectOutline(RectF rect, RenderColour colour);
void debugDrawRect(RectF rect, RenderColour colour);
void debugDrawRects(std::vector<RectF> rects, RenderColour colour);
void debugRenderText(const std::string text, int ptSize, VectorF position);