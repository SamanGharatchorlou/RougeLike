#include "pch.h"
#include"Map/Map.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "MapGenerator.h"

#include "MapTileDecoder.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


Map::Map(Vector2D<int> mapIndexSize, VectorF tileSize) : MapBase(mapIndexSize), mTileSize(tileSize) { }


void Map::populateData(TextureManager* tm, VectorF offset)
{
	populateTileRects(offset);

	MapTileDecoder decoder(tm);
	decoder.populateData(mData);
}


void Map::close(TextureManager* tm)
{
	// Carve out path
	for (int y = 0; y < yCount(); y++)
	{
		for (int x = 0; x < 2; x++)
		{
			Index index(x, y);
			mData[index].set(RenderTile::Wall);
		}
	}

	MapTileDecoder decoder(tm);
	decoder.populateData(mData);
}


void Map::renderLowerLayer()
{
	Camera* camera = Camera::Get();
	for (unsigned int x = 0; x < xCount(); x++)
	{
		bool onTopSection = true;

		for (unsigned int y = 0; y < yCount(); y++)
		{
			MapTile tile = mData[y][x];

			if (tile.has(RenderTile::Floor))
				onTopSection = false;

			if (!onTopSection)
			{
				// Move to next x
				onTopSection = true;
				break;
			}

			RectF tileRect = tile.rect();

			if (camera->inView(tileRect))
			{
				tileRect = camera->toCameraCoords(tileRect);
				tile.render(tileRect);
			}
		}
	}
}


void Map::renderUpperLayer()
{
	Camera* camera = Camera::Get();

	for (unsigned int x = 0; x < xCount(); x++)
	{
		bool onBottomSection = true;

		for (unsigned int y = yCount() - 1; y > 0; y--)
		{
			MapTile tile = mData[y][x];

			if (tile.has(RenderTile::Floor))
				onBottomSection = false;

			if (!onBottomSection)
			{
				// Move to next x
				onBottomSection = true;
				break;
			}

			RectF tileRect = tile.rect();

			if (camera->inView(tileRect))
			{
				tileRect = camera->toCameraCoords(tileRect);
				tile.render(tileRect);
			}
		}
	}

#if LABEL_SURFACE_RENDER_TYPES || LABEL_SURFACE_COLLISION_TYPES
	renderSurfaceTypes();
#endif
}


void Map::clearData()
{
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			MapTile& tile = mData[Index(x, y)];

			tile.setTexture(nullptr);
			tile.set(RenderTile::None);
			tile.set(CollisionTile::None);
			tile.setRect(RectF());
		}
	}
}


// --- Getters --- //
const Index Map::index(VectorF position) const
{
	VectorF mapTopLeft = mData.get(Index(0, 0)).rect().TopLeft();
	VectorF shiftedPosition = position - mapTopLeft;
	return isValidPosition(position) ? Index(shiftedPosition / tileSize()) : Index(-1, -1);
}


const Index Map::index(const MapTile* tile) const
{
	VectorF position(tile->rect().TopLeft());
	return index(position);
}


const Index Map::index(RectF rect) const
{               
	VectorF position(rect.TopLeft());
	return index(position);
}


const MapTile* Map::tile(VectorF position) const 
{
	Index tileIndex = index(position);
	return isValidIndex(tileIndex) ? &mData.get(tileIndex) : nullptr;
}


const RectF Map::getFirstRect() const
{
	return mData.get(Index(0, 0)).rect();
}


const RectF Map::getLastRect() const
{
	return  mData.get(Index(xCount() - 1, 0)).rect();
}



// -- Validity functions -- //
bool Map::isValidTile(RectF rect) const
{
	VectorF start = mData.get(Index(0, 0)).rect().TopLeft();
	VectorF end = mData.get(Index(xCount(), yCount()) - 1).rect().BotRight();

	return (rect.x1 >= start.x && rect.y1 >= start.y) &&
			(rect.x2 < end.x && rect.y2 < end.y) &&
			 rect.Size() == tileSize();
}


bool Map::isValidPosition(VectorF position) const
{
	VectorF start = mData.get(Index(0, 0)).rect().TopLeft();
	VectorF end = mData.get(Index(xCount(), yCount()) - 1).rect().BotRight();

	return (position.x >= start.x && position.x < end.x) &&
			(position.y >= start.y && position.y < end.y);
}


/// --- Private Function --- ///
void Map::populateTileRects(VectorF offset)
{
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			VectorF position = VectorF(x * mTileSize.x, y * mTileSize.y);
			RectF rect(position + offset, mTileSize);
			mData[y][x].setRect(rect);
		}
	}
}

void Map::renderFloor()
{
	Camera* camera = Camera::Get();

	for (unsigned int y = 0; y < yCount(); y++)
	{
		for (unsigned int x = 0; x < xCount(); x++)
		{
			MapTile tile = mData[y][x];
			RectF tileRect = tile.rect();

			if (camera->inView(tileRect))
			{

				RenderTile type = tile.renderType();

				// Split each floor tile into 4
				if (tile.has(RenderTile::Floor) || type >= RenderTile::Water_Middle)
				{
					tileRect = camera->toCameraCoords(tileRect);

					//if (tile.hasRenderType(MapTile::Floor_Small))
					//{
					//	// Split tile into 4 pieces
					//	VectorF size = tileRect.Size() / 2.0f;
					//	tileRect.SetSize(size);

					//	tile.render(tileRect);
					//	tile.render(tileRect.Translate(VectorF(tileRect.Size().x, 0.0f)));
					//	tile.render(tileRect.Translate(VectorF(tileRect.Size().x, tileRect.Size().x)));
					//	tile.render(tileRect.Translate(VectorF(0.0f, tileRect.Size().x)));
					//}
					//else
					{
						tile.render(tileRect);
					}
				}
			}
		}
	}
}


// -- Debugging --
#if LABEL_SURFACE_RENDER_TYPES || LABEL_SURFACE_COLLISION_TYPES
void Map::renderSurfaceTypes()
{
	if (frames++ % 30 == 0)
		frames = 0;
	else
		return;

	Camera* camera = Camera::Get();

	int fontSize = 16;
	VectorF offset = VectorF(0.0f, 20.0f);

	for (unsigned int y = 0; y < yCount(); y++)
	{
		for (unsigned int x = 0; x < xCount(); x++)
		{
			MapTile tile = mData[y][x];
			RectF tileRect = tile.rect();

			if (camera->inView(tileRect))
			{
				debugDrawRectOutline(Camera::Get()->toCameraCoords(tileRect), RenderColour::Green);

#if LABEL_SURFACE_RENDER_TYPES
				if (tile.has(RenderTile::Wall))
				{
					debugRenderText("Wall", fontSize, tileRect.TopCenter(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				// Floor
				if (tile.has(RenderTile::Floor))
				{
					debugRenderText("Floor", fontSize, tileRect.TopCenter(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				// Floor - Sides
				if (tile.has(RenderTile::Floor_Left))
				{
					debugRenderText("Floor left", fontSize, tileRect.TopCenter(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Floor_Right))
				{
					debugRenderText("Floor right", fontSize, tileRect.TopCenter(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Floor_Top))
				{
					debugRenderText("Floor top", fontSize, tileRect.TopCenter(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Floor_Bottom))
				{
					debugRenderText("Floor bottom", fontSize, tileRect.TopCenter(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				// Floor - Corners
				if (tile.has(RenderTile::Floor_Top_Left))
				{
					debugRenderText("Floor top left", fontSize, tileRect.TopCenter(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}
				if (tile.has(RenderTile::Floor_Top_Right))
				{
					debugRenderText("Floor top right", fontSize, tileRect.TopCenter(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}
				if (tile.has(RenderTile::Floor_Bottom_Left))
				{
					debugRenderText("Floor bottom left", fontSize, tileRect.TopCenter(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}
				if (tile.has(RenderTile::Floor_Bottom_Right))
				{
					debugRenderText("Floor bottom right", fontSize, tileRect.TopCenter(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				// Bottom walls
				if (tile.has(RenderTile::Bottom_Lower))
				{
					debugRenderText("Bottom lower", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Bottom_Upper))
				{
					debugRenderText("Bottom upper", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				// Top walls
				if (tile.has(RenderTile::Top_Lower))
				{
					debugRenderText("Top lower", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Top_Upper))
				{
					debugRenderText("Top upper", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				// Side walls
				if (tile.has(RenderTile::Right))
				{
					debugRenderText("Right", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Left))
				{
					debugRenderText("Left", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Bottom))
				{
					debugRenderText("Bottom", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				// Corners
				if (tile.has(RenderTile::Bottom_Right))
				{
					debugRenderText("Bottom right", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Bottom_Left))
				{
					debugRenderText("Bottom left", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Top_Right))
				{
					debugRenderText("Top right", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Top_Left))
				{
					debugRenderText("Top left", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				// Points
				if (tile.has(RenderTile::Point_Bottom_Right))
				{
					debugRenderText("bot right point", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Point_Bottom_Left))
				{
					debugRenderText("bot left point", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Point_Top_Right))
				{
					debugRenderText("top right point", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Point_Top_Left))
				{
					debugRenderText("top left point", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				// Column parts
				if (tile.has(RenderTile::Column_Upper))
				{
					debugRenderText("column upper", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Column_Lower))
				{
					debugRenderText("column lower", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Floor_ColumnBase))
				{
					debugRenderText("floor column base", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				// Water				
				if (tile.has(RenderTile::Water_Middle))
				{
					debugRenderText("Water", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.has(RenderTile::Water_Top))
				{
					debugRenderText("Water top", fontSize, tileRect.Center(), RenderColour::Red);
					tileRect = tileRect.Translate(offset);
				}
#endif // LABEL_SURFACE_RENDER_TYPES

#if LABEL_SURFACE_COLLISION_TYPES
				if (tile.is(CollisionTile::Floor))
				{
					debugRenderText("Floor", fontSize, tileRect.Center(), RenderColour::Blue);
					tileRect = tileRect.Translate(offset);
				}	

				if (tile.is(CollisionTile::Wall))
				{
					debugRenderText("Wall", fontSize, tileRect.Center(), RenderColour::Blue);
					tileRect = tileRect.Translate(offset);
				}

				if (tile.is(CollisionTile::Water))
				{
					debugRenderText("Water", fontSize, tileRect.Center(), RenderColour::Blue);
					tileRect = tileRect.Translate(offset);
				}
#endif  // LABEL_SURFACE_COLLISION_TYPES
			}
		}
	}
}
#endif 

