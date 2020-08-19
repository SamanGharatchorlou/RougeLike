#pragma once

#include "Utilities/RenderColour.h"


void debugDrawLine(VectorF pointA, VectorF pointB, RenderColour colour);
void debugDrawRectOutline(RectF rect, RenderColour colour);
void debugDrawRect(RectF rect, RenderColour colour);
void debugDrawRects(std::vector<RectF> rects, RenderColour colour);
void debugRenderText(const BasicString text, int ptSize, VectorF position, RenderColour colour, const BasicString& alignment = "Center");