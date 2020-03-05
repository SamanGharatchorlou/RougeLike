#include "pch.h"
#include"Map/Map.h"

#include "Game/GameData.h"
#include "Game/Camera.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "ProceduralMap/TunnelGenerator.h"


Map::Map(GameData* gameData) : mGameData(gameData), mScale(1.0f) , yLayerA(0.0f)
{
	mTileSize.set(16, 16); // TODO: get this from some where else?
}

void Map::populateTileRects()
{
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			VectorF size(mTileSize);
			size = size * mScale;
			VectorF position = VectorF(x * size.x, y * size.y);
			RectF rect(position, size);

			mData[y][x].setRect(rect);

#if _DEBUG
			mData[y][x].index = Vector2D<int>(x,y);
#endif
		}
	}

	yLayerA = size().y;
}


void Map::generateRandomTunnel(int y, int x)
{
	mTileCount.set(x, y);

	// clear data, default all tiles to walls
	mData.clearAndSet(x, y, MapTile());

	TunnelGenerator generator(mData);
	generator.build();

	populateTileRects();

	for (unsigned int x = 0; x < xCount(); x++)
	{
		bool floorAboveReached = false;

		for (unsigned int y = 0; y < yCount(); y++)
		{
			// query surronding tiles
			const MapTile::EdgeInfo& info = getEdgeInfo(x, y);
			MapTile& tile = mData[y][x];

			if (info.hasEdge)
			{
				// floor left
				if (info.data[1][0] == MapTile::Floor)
				{
					tile.addRenderType(MapTile::Left);
					tile.addCollisionType(MapTile::Left);
				}

				// floor right
				if (info.data[1][2] == MapTile::Floor)
				{
					tile.addRenderType(MapTile::Right);
					tile.addCollisionType(MapTile::Right);
				}


				// Only left and right needs to be considered once true
				if (floorAboveReached)
				{
					tile.removeCollisionType(MapTile::Floor);
					tile.addCollisionType(MapTile::Wall);
					continue;
				}

				// floor above
				if (info.data[0][1] == MapTile::Floor)
				{
					// Editing the tile below messes with the looping
					// only edit the sides after this has been hit
					floorAboveReached = true;

					tile.addRenderType(MapTile::Top);

					if (inBounds(x, y + 1) && wallRenderTile(x, y + 1))
					{
						MapTile::Type tileType = tile.collisionType();

						mData[y + 1][x].setCollisionType(tileType ^ MapTile::Top);

						int a = 0;
					}

					tile.setCollisionType(MapTile::Floor);
				}

				// floor below
				if (info.data[2][1] == MapTile::Floor)
				{
					tile.addRenderType(MapTile::Bot);
					tile.addCollisionType(MapTile::Bot);

					tile.removeRenderType(MapTile::Left ^ MapTile::Right);

					// Add isometic wall edges if the left/right tile is a wall but is not a MapTile::Bot
					if (inBounds(x-1,y) && wallRenderTile(x - 1, y) && !(mData[y][x - 1].hasRenderType(MapTile::Bot)))
					{
						mData[y][x - 1].addRenderType(MapTile::Right);
					}

					if (inBounds(x+1,y) && wallRenderTile(x + 1, y) && !(mData[y][x + 1].hasRenderType(MapTile::Bot)))
					{
						mData[y][x + 1].addRenderType(MapTile::Left);
					}
				}
			}
		}
	}

#if DRAW_BINARY_MAP
	printBinaryMap();
#endif
}


Vector2D<float> Map::size() const
{
	return Vector2D<float>(xCount(), yCount()) * mTileSize * mScale;
}


void Map::renderLayerA(float yPoint)
{
#if _DEBUG
	tileRenderCounter = 0;
#endif

	yLayerA = yPoint;

	TextureManager* tm = mGameData->textureManager;

	Texture* floor = tm->getTexture("floor");
	Texture* wall = tm->getTexture("wall");

	Texture* leftEdge = tm->getTexture("wall_left_edge");
	Texture* rightEdge = tm->getTexture("wall_right_edge");
	Texture* botEdge = tm->getTexture("wall_bot_edge");

	Camera* camera = Camera::Get();
	Vector2D<int> size = mTileSize * mScale;

	for (unsigned int y = 0; y < yCount(); y++)
	{
		for (unsigned int x = 0; x < xCount(); x++)
		{
			Rect<int> tileRect(Vector2D<int>(x, y) * size, size);

			if (camera->inView(tileRect))
			{
				MapTile tile = mData[y][x];

				// Render walls 'below/under' the player after the player 
				if (tile.rect().Center().y >= yLayerA && !tile.hasRenderType(MapTile::Floor))
				{
					continue;
				}

				tileRect = camera->toCameraCoords(tileRect);

				if (mData[y][x].hasRenderType(MapTile::Wall))
					wall->render(tileRect);
				else
					floor->render(tileRect);

				if (mData[y][x].hasRenderType(MapTile::Left))
					leftEdge->render(tileRect);

				if (mData[y][x].hasRenderType(MapTile::Right))
					rightEdge->render(tileRect);

				if (mData[y][x].hasRenderType((MapTile::Top ^ MapTile::Bot)))
					botEdge->render(tileRect, SDL_FLIP_VERTICAL);

#if _DEBUG
				tileRenderCounter++;
#endif
			}
		}
	}
}

void Map::renderLayerB()
{
	TextureManager* tm = mGameData->textureManager;

	Texture* floor = tm->getTexture("floor");
	Texture* wall = tm->getTexture("wall");

	Texture* leftEdge = tm->getTexture("wall_left_edge");
	Texture* rightEdge = tm->getTexture("wall_right_edge");
	Texture* botEdge = tm->getTexture("wall_bot_edge");

	Camera* camera = Camera::Get();
	Vector2D<int> size = mTileSize * mScale;

	for (unsigned int y = 0; y < yCount(); y++)
	{
		for (unsigned int x = 0; x < xCount(); x++)
		{
			Rect<int> tileRect(Vector2D<int>(x, y) * size, size);

			if (camera->inView(tileRect))
			{
				MapTile tile = mData[y][x];

				// skip anything that would have been rendered in layer A
				if (tile.rect().Center().y < yLayerA || tile.hasRenderType(MapTile::Floor))
				{
					continue;
				}

				tileRect = camera->toCameraCoords(tileRect);

				if (mData[y][x].hasRenderType(MapTile::Wall))
					wall->render(tileRect);
				else
					floor->render(tileRect);

				if (mData[y][x].hasRenderType(MapTile::Left))
					leftEdge->render(tileRect);

				if (mData[y][x].hasRenderType(MapTile::Right))
					rightEdge->render(tileRect);

				if (mData[y][x].hasRenderType((MapTile::Top ^ MapTile::Bot)))
					botEdge->render(tileRect, SDL_FLIP_VERTICAL);

#if _DEBUG
				tileRenderCounter++;
#endif
			}
		}
	}

#if _DEBUG
	float tilesInCamera = (Camera::Get()->size() / getTileSize()).area();

	if (tileRenderCounter > tilesInCamera + (yCount() * 2) + (xCount() * 2))
		DebugPrint(Log, "There are approx %f tiles within the viewport and we are rendering %f, too many?\n",
			tilesInCamera + (yCount() * 2) + (xCount() * 2), tileRenderCounter);
#endif
}


const MapTile* Map::offsetTile(const MapTile* tile, int xOffset, int yOffset) const
{
	Vector2D<int> index = getIndex(tile);
	index = index + Vector2D<int>(xOffset, yOffset);

	return isValidIndex(index) ? getTile(index) : nullptr;
}


Vector2D<int> Map::findYFloorTileRange(int xTileIndex)
{
	unsigned int yTileIndex = 0;
	Vector2D<unsigned int> yTileRange;

	while (wallCollisionTile(xTileIndex, ++yTileIndex)) {}

	// highest point
	yTileRange.x = yTileIndex;

	while (floorCollisionTile(xTileIndex, ++yTileIndex))
	{
		if (yTileIndex >= yCount() - 1)
			break;
	}

	// remove last increment to keep within floor and minus one extra to prevent enemy moving behind the wall
	yTileRange.y = clamp(yTileIndex - 2, yTileRange.x, yCount() - 2);

	return yTileRange;
}


bool Map::inBounds(int x, int y) const
{
	bool xBounds = (x >= 0 && x < mTileCount.x);
	bool yBounds = (y >= 0 && y < mTileCount.y);

	return xBounds && yBounds;
}


// --- Getters --- //
const MapTile::EdgeInfo Map::getEdgeInfo(int x, int y) const
{
	MapTile::EdgeInfo info;

	// Is a wall tile
	if (wallRenderTile(x, y))
	{
		for (int j = y - 1; j <= y + 1; j++)
		{
			for (int i = x - 1; i <= x + 1; i++)
			{
				// If touching a floor tile
				if (inBounds(i, j) && floorRenderTile(i, j))
				{
					info.data[j - y + 1][i - x + 1] = MapTile::Floor;
					info.hasEdge = true;
				}
			}
		}
	}

	return info;
}


const Vector2D<int> Map::getIndex(const VectorF position) const
{
	Vector2D<int> index(position.x / (mTileSize.x * mScale), position.y / (mTileSize.y * mScale));
	return isValidPosition(position) ? index : Vector2D<int>(-1, -1);
}


const Vector2D<int> Map::getIndex(const MapTile* tile) const
{
	const VectorF position(tile->rect().TopLeft());
	Vector2D<int> index(position.x / (mTileSize.x * mScale), position.y / (mTileSize.y * mScale));
	return isValidPosition(position) ? index : Vector2D<int>(-1, -1);
}


const Vector2D<int> Map::getIndex(RectF rect) const
{
	if (isValidTile(rect))
		return Vector2D<int>(rect.Center().x, rect.Center().y) / (mTileSize * mScale);
	else
		return Vector2D<int>(-1, -1);
}


const MapTile* Map::getTile(const Vector2D<int> index) const
{
	return isValidIndex(index) ? &mData.get(index) : nullptr;
}


const MapTile* Map::getTile(int x, int y) const
{
	return isValidIndex(Vector2D<int>(x,y)) ? &mData.get(x,y) : nullptr;
}


const MapTile* Map::getTile(VectorF position) const 
{
	Vector2D<int> tileIndex(position.x / (mTileSize.x * mScale), position.y / (mTileSize.y * mScale));
	return isValidIndex(tileIndex) ? &mData.get(tileIndex) : nullptr;
}


const RectF Map::getTileRect(Vector2D<int> index) const
{
	VectorF size = VectorF(mTileSize.x, mTileSize.y) * mScale;
	VectorF position = VectorF(index.x, index.y) * size;
	RectF rect(position, size);

	return isValidTile(rect) ? rect : RectF(-1);
}


const RectF Map::getTileRect(int x, int y) const
{
	VectorF size = VectorF(mTileSize.x, mTileSize.y) * mScale;
	VectorF position = VectorF(x, y) * size;
	RectF rect(position, size);

	return isValidTile(rect) ? rect : RectF(-1);
}


const RectF Map::getTileRect(int coords[2]) const
{
	VectorF size = VectorF(mTileSize.x, mTileSize.y) * mScale;
	VectorF position = VectorF(coords[0], coords[1]) * size;
	RectF rect(position, size);

	return isValidTile(rect) ? rect : RectF(-1);
}


// -- Validity functions -- //
bool Map::isValidTile(RectF rect) const
{
#if _DEBUG
	bool isValid = rect.x1 >= 0.0f && rect.y1 >= 0.0f &&
					rect.x2 < size().x && rect.y2 < size().y &&
					rect.Size() == mTileSize * mScale;
	if (!isValid)
	{
		DebugPrint(Warning, "Rect topleft p(%f,%f), s(%f,%f) is invalid\n",
			rect.TopLeft().x, rect.TopLeft().y, rect.Size().x, rect.Size().y);
	}
	return isValid;
#else // if release
	return (
		rect.x1 >= 0.0f && rect.y1 >= 0.0f &&
		rect.x2 < size().x && rect.y2 < size().y &&
		rect.Size() == mTileSize * mScale);
#endif
}


bool Map::isValidIndex(Vector2D<int> index) const
{
#if _DEBUG
	bool isValid = (index.x >= 0 && index.x < mTileCount.x) &&
					(index.y >= 0 && index.y < mTileCount.y);
	if (!isValid)
	{
		DebugPrint(Warning, "index (%d,%d), is out of bounds\n", index.x, index.y);
	}
	return isValid;
#else // if release
	return (index.x >= 0 && index.x < mTileCount.x) &&
		(index.y >= 0 && index.y < mTileCount.y);
#endif
}


bool Map::isValidPosition(VectorF position) const
{
#if _DEBUG
	bool isValid = position.x >= 0 && position.x < (float)size().x &&
					position.y >= 0 && position.y < (float)size().y;
	if (!isValid)
	{
		DebugPrint(Warning, "position(%f,%f) is invalid\n", position.x, position.y);
	}
	return isValid;
#else
	return position.x >= 0 && position.x < (float)size().x &&
		position.y >= 0 && position.y < (float)size().y;
#endif
}



// --- Debugging --- //
#if DRAW_BINARY_MAP
void Map::printBinaryMap()
{
	int cols = mData.xCount();
	int rows = mData.yCount();

	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			DebugPrint(Log, "%d ", mData[y][x].renderType());
		}

		DebugPrint(Log, "\n");
	}
}
#endif