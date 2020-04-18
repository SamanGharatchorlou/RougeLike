#pragma once
#include "pch.h"
#include "DebugDraw.h"
#include "Game/Camera.h"


void debugDrawLine(VectorF pointA, VectorF pointB, RenderColour colour)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

	Vector2D<int> A = Camera::Get()->toCameraCoords(pointA);
	Vector2D<int> B = Camera::Get()->toCameraCoords(pointB);

	SDL_RenderDrawLine(Renderer::Get()->sdlRenderer(), A.x, A.y, B.x, B.y);
}


void debugDrawRect(RectF rect, RenderColour colour)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

	RectF rectb = Camera::Get()->toCameraCoords(rect);
	SDL_Rect renderQuadb = { static_cast<int>(rectb.x1),
					static_cast<int>(rectb.y1),
					static_cast<int>(rectb.Width()),
					static_cast<int>(rectb.Height()) };

	SDL_RenderFillRect(Renderer::Get()->sdlRenderer(), &renderQuadb);
}


void debugDrawRects(std::vector<RectF> rects, RenderColour colour)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);
	
	for (unsigned int i = 0; i < rects.size(); i++)
	{
		RectF rectb = Camera::Get()->toCameraCoords(rects[i]);
		SDL_Rect renderQuadb = { static_cast<int>(rectb.x1),
						static_cast<int>(rectb.y1),
						static_cast<int>(rectb.Width()),
						static_cast<int>(rectb.Height()) };

		SDL_RenderFillRect(Renderer::Get()->sdlRenderer(), &renderQuadb);
	}
}