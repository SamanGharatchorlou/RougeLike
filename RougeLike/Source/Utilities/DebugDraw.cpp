#pragma once
#include "pch.h"
#include "DebugDraw.h"
#include "Game/Camera.h"
#include "Game/GameData.h"

void debugDrawRect(GameData* gameData, RectF rect, RenderColour colour)
{
	SDL_SetRenderDrawColor(gameData->renderer, colour.r, colour.g, colour.b, colour.a);

	RectF rectb = gameData->camera->toCameraCoords(rect);
	SDL_Rect renderQuadb = { static_cast<int>(rectb.x1),
					static_cast<int>(rectb.y1),
					static_cast<int>(rectb.Width()),
					static_cast<int>(rectb.Height()) };

	SDL_RenderFillRect(gameData->renderer, &renderQuadb);
}

void debugDrawRects(GameData* gameData, std::vector<RectF> rects, RenderColour colour)
{
	SDL_SetRenderDrawColor(gameData->renderer, colour.r, colour.g, colour.b, colour.a);
	for (unsigned int i = 0; i < rects.size(); i++)
	{
		RectF rectb = gameData->camera->toCameraCoords(rects[i]);
		SDL_Rect renderQuadb = { static_cast<int>(rectb.x1),
						static_cast<int>(rectb.y1),
						static_cast<int>(rectb.Width()),
						static_cast<int>(rectb.Height()) };

		SDL_RenderFillRect(gameData->renderer, &renderQuadb);
	}
}