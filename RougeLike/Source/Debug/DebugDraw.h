#pragma once

#include "Utilities/RenderColour.h"
#include "Utilities/Quad2D.h"

void debugDrawPoint(VectorF point, float size, RenderColour colour);
void debugDrawLine(VectorF pointA, VectorF pointB, RenderColour colour);
void debugDrawRectOutline(RectF rect, RenderColour colour);
void debugDrawRect(RectF rect, RenderColour colour);
void debugDrawRects(std::vector<RectF> rects, RenderColour colour);
void debugDrawQuad(Quad2D<float> quad, RenderColour colour);
void debugRenderText(const BasicString text, int ptSize, VectorF position, RenderColour colour, const BasicString& alignment = "Center");