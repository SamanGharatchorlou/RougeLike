#include "pch.h"
#include "MapDebugging.h"

#if _DEBUG
#include "Game/Camera.h"
#include "Debug/DebugDraw.h"

void renderSurfaceTypes(const Grid<MapTile>& data)
{
	Camera* camera = Camera::Get();

	int fontSize = 16;
	VectorF offset = VectorF(0.0f, 20.0f);

	for (unsigned int y = 0; y < data.yCount(); y++)
	{
		for (unsigned int x = 0; x < data.xCount(); x++)
		{
			Index index(x, y);
			MapTile tile = data.get(index);
			RectF tileRect = tile.rect();

			if (camera->inView(tileRect))
			{
				debugDrawRectOutline(Camera::Get()->toCameraCoords(tileRect), RenderColour::Green);

#if LABEL_SURFACE_COLLISION_TYPES
				renderCollisionTypes(tile, tileRect, offset, fontSize);
#endif

#if LABEL_SURFACE_RENDER_TYPES
				renderRenderTypes(tile, tileRect, offset, fontSize);
#endif

#if LABEL_SURFACE_DECOR_TYPES
				renderDecorTypes(tile, tileRect, offset, fontSize);
#endif
			}
		}
	}
}

void renderCollisionTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize)
{
	RenderColour colour = RenderColour::Red;

	if (tile.has(RenderTile::Wall))
	{
		debugRenderText("Wall", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Floor
	if (tile.has(RenderTile::Floor))
	{
		debugRenderText("Floor", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Floor - Sides
	if (tile.has(RenderTile::Floor_Left))
	{
		debugRenderText("Floor left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Floor_Right))
	{
		debugRenderText("Floor right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Floor_Top))
	{
		debugRenderText("Floor top", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Floor_Bottom))
	{
		debugRenderText("Floor bottom", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Floor - Corners
	if (tile.has(RenderTile::Floor_Top_Left))
	{
		debugRenderText("Floor top left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
	if (tile.has(RenderTile::Floor_Top_Right))
	{
		debugRenderText("Floor top right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
	if (tile.has(RenderTile::Floor_Bottom_Left))
	{
		debugRenderText("Floor bottom left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
	if (tile.has(RenderTile::Floor_Bottom_Right))
	{
		debugRenderText("Floor bottom right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Bottom walls
	if (tile.has(RenderTile::Bottom_Lower))
	{
		debugRenderText("Bottom lower", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Bottom_Upper))
	{
		debugRenderText("Bottom upper", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Top walls
	if (tile.has(RenderTile::Top_Lower))
	{
		debugRenderText("Top lower", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Top_Upper))
	{
		debugRenderText("Top upper", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Side walls
	if (tile.has(RenderTile::Right))
	{
		debugRenderText("Right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Left))
	{
		debugRenderText("Left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Bottom))
	{
		debugRenderText("Bottom", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Corners
	if (tile.has(RenderTile::Bottom_Right))
	{
		debugRenderText("Bottom right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Bottom_Left))
	{
		debugRenderText("Bottom left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Top_Right))
	{
		debugRenderText("Top right", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Top_Left))
	{
		debugRenderText("Top left", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Points
	if (tile.has(RenderTile::Point_Bottom_Right))
	{
		debugRenderText("bot right point", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Point_Bottom_Left))
	{
		debugRenderText("bot left point", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Point_Top_Right))
	{
		debugRenderText("top right point", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Point_Top_Left))
	{
		debugRenderText("top left point", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Column parts
	if (tile.has(RenderTile::Column_Upper))
	{
		debugRenderText("column upper", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Column_Lower))
	{
		debugRenderText("column lower", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Floor_ColumnBase))
	{
		debugRenderText("floor column base", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	// Water				
	if (tile.has(RenderTile::Water_Middle))
	{
		debugRenderText("Water middle", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(RenderTile::Water_Top))
	{
		debugRenderText("Water top", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
}
void renderRenderTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize)
{
	RenderColour colour = RenderColour::Blue;

	if (tile.is(CollisionTile::Floor))
	{
		debugRenderText("Floor", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.is(CollisionTile::Wall))
	{
		debugRenderText("Wall", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.is(CollisionTile::Water))
	{
		debugRenderText("Water", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.is(CollisionTile::Column))
	{
		debugRenderText("Column", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
}


void renderDecorTypes(const MapTile& tile, RectF& tileRect, VectorF& offset, int fontSize)
{
	RenderColour colour = RenderColour::Green;

	if (tile.has(DecorTile::Water))
	{
		debugRenderText("Water", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(DecorTile::Column))
	{
		debugRenderText("Column", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(DecorTile::Torch))
	{
		debugRenderText("Torch", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}

	if (tile.has(DecorTile::Spikes))
	{
		debugRenderText("Spikes", fontSize, tileRect.TopCenter(), colour);
		tileRect = tileRect.Translate(offset);
	}
}

#endif // DEBUG