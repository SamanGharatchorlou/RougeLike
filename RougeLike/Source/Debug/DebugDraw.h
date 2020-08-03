#pragma once

struct RenderColour
{
	enum Colour
	{
		None,
		Red,
		Blue,
		Green,
		Yellow,
		LightGrey,
		Black
	};

	//RenderColour() { }
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
		case RenderColour::Black:
			r = 0; g = 0; b = 0;
			break;
		default:
			DebugPrint(Log, "Render colour has not been defined\n");
			break;
		}
	}

	void setOpacity(float opacity)
	{
		opacity = clamp(opacity, 0.0f, 1.0f);
		a = (Uint8)(255 * opacity);
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
void debugRenderText(const BasicString text, int ptSize, VectorF position, RenderColour colour, const BasicString& alignment = "Center");