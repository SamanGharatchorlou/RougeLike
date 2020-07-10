#include "pch.h"
#include"Map/Map.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "MapGenerator.h"

#include "MapTileDecoder.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#include "MapDebugging.h"
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


void Map::slowUpdate(float dt)
{
	for (int y = 0; y < yCount(); y++)
	{
		for (int x = 0; x < xCount(); x++)
		{
			Index index(x, y);
			mData[index].slowUpdate(dt);
		}
	}

	mTrapManager.slowUpdate(this);
}


void Map::renderLowerLayer()
{
	Camera* camera = Camera::Get();
	for (unsigned int x = 0; x < xCount(); x++)
	{
		for (unsigned int y = 0; y < yCount(); y++)
		{
			Index index(x, y);
			MapTile tile = mData[index];

			if(tile.is(CollisionTile::Floor))
				break;

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
		for (unsigned int y = yCount() - 1; y > 0; y--)
		{
			Index index(x, y);
			MapTile tile = mData[index];

			if (tile.renderType() < RenderTile::Wall)
				break;

			RectF tileRect = tile.rect();
			if (camera->inView(tileRect))
			{
				tileRect = camera->toCameraCoords(tileRect);
				tile.render(tileRect);
			}
		}
	}

#if RENDER_SURFACE_TYPES
	renderSurfaceTypes(mData);
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
				//if (tile.has(RenderTile::Floor) || type >= RenderTile::Water_Middle)
				if(tile.is(CollisionTile::Floor))
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

