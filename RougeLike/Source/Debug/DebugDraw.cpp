#pragma once
#include "pch.h"
#include "DebugDraw.h"

#include "Game/Camera/Camera.h"
#include "UI/Elements/UITextBox.h"
#include "Graphics/Renderer.h"



void debugDrawPoint(VectorF point, float size, RenderColour colour)
{
	RectF rect;
	rect.SetSize(size, size);
	rect.SetCenter(point);

	debugDrawRect(rect, colour);
}


void debugDrawLine(VectorF pointA, VectorF pointB, RenderColour colour)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

	Vector2D<int> A = Camera::Get()->toCameraCoords(pointA).toInt();
	Vector2D<int> B = Camera::Get()->toCameraCoords(pointB).toInt();

	SDL_RenderDrawLine(Renderer::Get()->sdlRenderer(), A.x, A.y, B.x, B.y);
}


void debugDrawRectOutline(RectF rect, RenderColour colour, bool translateToCameraCoords)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

	if (translateToCameraCoords)
	{
		rect = Camera::Get()->toCameraCoords(rect);
	}

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


void debugDrawQuad(Quad2D<float> quad, RenderColour colour)
{
	SDL_SetRenderDrawColor(Renderer::Get()->sdlRenderer(), colour.r, colour.g, colour.b, colour.a);

	for (unsigned int i = 0; i < quad.sides(); i++)
	{
		quad[i] = Camera::Get()->toCameraCoords(quad[i]);
	}

	for (unsigned int i = 0; i < quad.sides(); i++)
	{
		int j = i + 1 >= quad.sides() ? 0 : i + 1;

		Vector2D<int> pointA = quad.at(i).toInt();
		Vector2D<int> pointB = quad.at(j).toInt();
		SDL_RenderDrawLine(Renderer::Get()->sdlRenderer(), pointA.x, pointA.y, pointB.x, pointB.y);
	}
}


void debugRenderText(const BasicString text, int ptSize, VectorF position, RenderColour colour, const BasicString& alignment)
{
	UITextBox::Data textData;
	textData.alignment = alignment;
	textData.ptSize = ptSize;
	textData.colour = SDL_Color{ colour.r, colour.g, colour.b };
	textData.rect = RectF(Camera::Get()->toCameraCoords(position), VectorF(0.0f,0.0f));
	textData.text = text;

	UITextBox textBox(textData);
	textBox.render();
}