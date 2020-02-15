#pragma once

#include "Attributes.h"
#include "Collisions/Collider.h"

struct Wall
{
	Wall(Attributes attributes)
	{
		// TODO: link with map scale
		float mapScale = 3.0f;

		float x = attributes.getFloat("object_x");
		float y = attributes.getFloat("object_y");

		float width = attributes.getFloat("object_width");
		float height = attributes.getFloat("object_height");

		rect.SetRect(VectorF(x, y) * mapScale, VectorF(width, height) * mapScale);

		collider.init(&rect);
	}

	RectF rect;
	Collider collider;
};