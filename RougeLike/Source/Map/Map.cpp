#include "pch.h"
#include"Map/Map.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "TunnelGenerator.h"





void Map::populateTileRects(VectorF offset)
{
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			VectorF size(VectorF(16.0f, 16.0f) * 3.0f); // TODO: hard coded
			VectorF position = VectorF(x * size.x, y * size.y);
			RectF rect(position + offset, size);

			mData[y][x].setRect(rect);

#if _DEBUG
			mData[y][x].index = Index(x,y);
#endif
		}
	}
}


void Map::populateCollisionRenderInfo()
{
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
					if (inBounds(x - 1, y) && wallRenderTile(x - 1, y) && !(mData[y][x - 1].hasRenderType(MapTile::Bot)))
					{
						mData[y][x - 1].addRenderType(MapTile::Right);
					}

					if (inBounds(x + 1, y) && wallRenderTile(x + 1, y) && !(mData[y][x + 1].hasRenderType(MapTile::Bot)))
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


void Map::init(int x, int y)
{
	mData.clearAndSet(x, y, MapTile());
}


void Map::populateData(VectorF offset)
{
	populateTileRects(offset);
	populateCollisionRenderInfo();
}


VectorF Map::size() const
{
	return VectorF(xCount(), yCount()) * tileSize();
}

const VectorF Map::tileSize() const
{ 
	return tile(0, 0)->rect().Size();
}


void Map::renderBottomLayer(const TextureManager* tm, float yPoint)
{
#if _DEBUG
	tileRenderCounter = 0;
#endif

	Texture* floor = tm->getTexture("floor");
	Texture* wall = tm->getTexture("wall");

	Texture* leftEdge = tm->getTexture("wall_left_edge");
	Texture* rightEdge = tm->getTexture("wall_right_edge");
	Texture* botEdge = tm->getTexture("wall_bot_edge");

	Texture* column = tm->getTexture("columnsmall");

	Camera* camera = Camera::Get();

	for (unsigned int y = 0; y < yCount(); y++)
	{
		for (unsigned int x = 0; x < xCount(); x++)
		{
			MapTile tile = mData[y][x];
			Rect<int> tileRect = tile.rect();

			if (camera->inView(tileRect))
			{
				// Render walls 'below/under' the player after the player 
				if (tileRect.Center().y >= yPoint && !tile.isRenderType(MapTile::Floor))
					continue;

				tileRect = camera->toCameraCoords(tileRect);

				if(tile.hasRenderType(MapTile::Floor))
					floor->render(tileRect);

				if (tile.hasRenderType(MapTile::ColumnBot))
					renderColumn(tileRect, column);

				if (tile.hasRenderType(MapTile::Wall))
					wall->render(tileRect);

				if (tile.hasRenderType(MapTile::Left))
					leftEdge->render(tileRect);

				if (tile.hasRenderType(MapTile::Right))
					rightEdge->render(tileRect);

				if (tile.hasRenderType(MapTile::Bot))
					botEdge->render(tileRect, SDL_FLIP_VERTICAL);

				if (tile.hasRenderType(MapTile::Top))
					botEdge->render(tileRect, SDL_FLIP_VERTICAL);

				if (tile.hasRenderType(MapTile::ColumnTop))
					renderColumn(tileRect, column);

#if _DEBUG
				tileRenderCounter++;
#endif
			}
		}
	}
}


void Map::renderTopLayer(const TextureManager* tm, float yPoint)
{
	Texture* floor = tm->getTexture("floor");
	Texture* wall = tm->getTexture("wall");

	Texture* leftEdge = tm->getTexture("wall_left_edge");
	Texture* rightEdge = tm->getTexture("wall_right_edge");
	Texture* botEdge = tm->getTexture("wall_bot_edge");

	Texture* column = tm->getTexture("columnsmall");

	Camera* camera = Camera::Get();

	for (unsigned int y = 0; y < yCount(); y++)
	{
		for (unsigned int x = 0; x < xCount(); x++)
		{
			MapTile tile = mData[y][x];
			Rect<int> tileRect = tile.rect();

			if (camera->inView(tileRect))
			{
				MapTile tile = mData[y][x];

				// skip anything that would have been rendered in layer A
				if (tile.rect().Center().y < yPoint || tile.isRenderType(MapTile::Floor))
					continue;

				tileRect = camera->toCameraCoords(tileRect);

				if (tile.hasRenderType(MapTile::Floor))
					floor->render(tileRect);

				if (tile.hasRenderType(MapTile::ColumnBot))
					renderColumn(tileRect, column);

				if (tile.hasRenderType(MapTile::Wall))
					wall->render(tileRect);

				if (tile.hasRenderType(MapTile::Left))
					leftEdge->render(tileRect);

				if (tile.hasRenderType(MapTile::Right))
					rightEdge->render(tileRect);

				if (tile.hasRenderType(MapTile::Bot))
					botEdge->render(tileRect, SDL_FLIP_VERTICAL);

				if (tile.hasRenderType(MapTile::Top))
					botEdge->render(tileRect, SDL_FLIP_VERTICAL);

				if (tile.hasRenderType(MapTile::ColumnTop))
					renderColumn(tileRect, column);

#if _DEBUG
				tileRenderCounter++;
#endif
			}
		}
	}

#if _DEBUG
	float tilesInCamera = (Camera::Get()->size() / tileSize()).area();

	if (tileRenderCounter > tilesInCamera + (yCount() * 2) + (xCount() * 2))
		DebugPrint(Log, "There are approx %f tiles within the viewport and we are rendering %f, too many?\n",
			tilesInCamera + (yCount() * 2) + (xCount() * 2), tileRenderCounter);
#endif
}


void Map::renderColumn(const RectF& rect, Texture* column)
{
	VectorF size = column->originalDimentions * 3.0f;
	VectorF position = rect.BotRight();

	RectF columnRect = RectF(position, size);
	columnRect.SetBotRight(position);

	column->render(columnRect);
}


const MapTile* Map::offsetTile(const MapTile* target, int xOffset, int yOffset) const
{
	Index tileIndex = index(target);
	tileIndex = tileIndex + Index(xOffset, yOffset);

	return isValidIndex(tileIndex) ? tile(tileIndex) : nullptr;
}


Index Map::findYFloorTileRange(int xTileIndex)
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


const Index Map::index(VectorF position) const
{
	VectorF mapTopLeft = mData.get(0, 0).rect().TopLeft();
	VectorF shiftedPosition = position - mapTopLeft;

	// Get the index relative to this map
	Index index(shiftedPosition.x / tileSize().x, shiftedPosition.y / tileSize().y);

	return isValidPosition(position) ? index : Index(-1, -1);
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


const RectF Map::tileRect(Index index) const
{
	return isValidIndex(index) ? mData.get(index).rect() : RectF(-1);
}


const RectF Map::tileRect(int x, int y) const
{
	return isValidIndex(Index(x, y)) ? mData.get(x, y).rect() : RectF(-1);
}



const RectF Map::getFirstRect(int yIndex) const
{
	return tileRect(0, yIndex);
}

const RectF Map::getLastRect(int yIndex) const
{
	return tileRect(xCount() - 1, yIndex);
}


void Map::addTileType(int x, int y, MapTile::Type type)
{
	mData[y][x].addRenderType(type);

	if (type >= MapTile::Wall)
		mData[y][x].setCollisionType(MapTile::Wall);
	else
		mData[y][x].setCollisionType(MapTile::Floor);
}


void Map::setTileType(int x, int y, MapTile::Type type)
{
	mData[y][x].setType(type);
}


void Map::removeTileType(int x, int y, MapTile::Type type)
{
	mData[y][x].addRenderType(type);

	if (type >= MapTile::Wall)
		mData[y][x].setCollisionType(MapTile::Wall);
	else
		mData[y][x].setCollisionType(MapTile::Floor);
}



// -- Validity functions -- //
bool Map::isValidTile(RectF rect) const
{
	VectorF start = mData.get(0, 0).rect().TopLeft();
	VectorF end = mData.get(xCount() - 1, yCount() - 1).rect().BotRight();

	return (rect.x1 >= start.x && rect.y1 >= start.y) &&
			(rect.x2 < end.x && rect.y2 < end.y) &&
			 rect.Size() == tileSize();
}


bool Map::isValidPosition(VectorF position) const
{
	VectorF start = mData.get(0, 0).rect().TopLeft();
	VectorF end = mData.get(xCount() - 1, yCount() - 1).rect().BotRight();

	return (position.x >= start.x && position.x < end.x) &&
			(position.y >= start.y && position.y < end.y);
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