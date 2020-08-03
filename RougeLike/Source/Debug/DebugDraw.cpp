#pragma once
#include "pch.h"
#include "DebugDraw.h"

#include "Game/Camera.h"
#include "UI/Elements/UITextBox.h"

void debugDrawLine(VectorF pointA, VectorF pointB, RenderColour colour)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

	Vector2D<int> A = Camera::Get()->toCameraCoords(pointA);
	Vector2D<int> B = Camera::Get()->toCameraCoords(pointB);

	SDL_RenderDrawLine(Renderer::Get()->sdlRenderer(), A.x, A.y, B.x, B.y);
}


void debugDrawRectOutline(RectF rect, RenderColour colour)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

	//RectF rectb = Camera::Get()->toCameraCoords(rect);
	SDL_Rect renderQuadb = { static_cast<int>(rect.x1),
					static_cast<int>(rect.y1),
					static_cast<int>(rect.Width()),
					static_cast<int>(rect.Height()) };

	SDL_RenderDrawRect(Renderer::Get()->sdlRenderer(), &renderQuadb);
}


void debugDrawRect(RectF rect, RenderColour colour)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

	rect = Camera::Get()->toCameraCoords(rect);
	SDL_Rect renderQuadb = { static_cast<int>(rect.x1),
					static_cast<int>(rect.y1),
					static_cast<int>(rect.Width()),
					static_cast<int>(rect.Height()) };

	SDL_RenderFillRect(Renderer::Get()->sdlRenderer(), &renderQuadb);
}


void debugDrawRects(std::vector<RectF> rects, RenderColour colour)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);
	
	for (unsigned int i = 0; i < rects.size(); i++)
	{
		RectF rect = Camera::Get()->toCameraCoords(rects[i]);
		SDL_Rect renderQuadb = { static_cast<int>(rect.x1),
						static_cast<int>(rect.y1),
						static_cast<int>(rect.Width()),
						static_cast<int>(rect.Height()) };

		SDL_RenderFillRect(Renderer::Get()->sdlRenderer(), &renderQuadb);
	}
}

void debugRenderText(const BasicString text, int ptSize, VectorF position, RenderColour colour, const BasicString& alignment)
{
	UITextBox::Data textData;
	textData.aligment = alignment;
	textData.font = "";
	textData.ptSize = ptSize;
	textData.colour = SDL_Color{ colour.r, colour.g, colour.b };
	textData.texture = nullptr;
	textData.rect = RectF(Camera::Get()->toCameraCoords(position), VectorF(0.0f,0.0f));
	textData.text = text;

	UITextBox textBox(textData);
	textBox.render();
}