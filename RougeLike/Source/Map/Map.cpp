#include "pch.h"
#include"Map/Map.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "TunnelGenerator.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif


Map::Map(Vector2D<int> mapIndexSize, VectorF tileSize) : MapBase(mapIndexSize, MapTile(MapTile::Wall)), mTileSize(tileSize) { }


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
				tileRect = camera->toCameraCoords(tileRect);

				// Split each floor tile into 4
				if (tile.hasRenderType(MapTile::Floor))
				{
					// Split tile into 4 pieces
					VectorF size = tileRect.Size() / 2.0f;
					tileRect.SetSize(size);

					tile.render(tileRect);
					tile.render(tileRect.Translate(VectorF(tileRect.Size().x, 0.0f)));
					tile.render(tileRect.Translate(VectorF(tileRect.Size().x, tileRect.Size().x)));
					tile.render(tileRect.Translate(VectorF(0.0f, tileRect.Size().x)));
				}
			}
		}
	}
}


void Map::renderTop()
{
	Camera* camera = Camera::Get();

	for (unsigned int x = 0; x < xCount(); x++)
	{
		bool onTopSection = true;

		for (unsigned int y = 0; y < yCount(); y++)
		{
			MapTile tile = mData[y][x];

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

			if (tile.hasRenderType(MapTile::Bottom_Lower))
				onTopSection = false;
		}
	}
}


void Map::renderBottom()
{
	Camera* camera = Camera::Get();

	for (unsigned int x = 0; x < xCount(); x++)
	{
		bool onBottomSection = true;

		for (unsigned int y = yCount() - 1; y > 0; y--)
		{
			MapTile tile = mData[y][x];

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

			if (tile.hasRenderType(MapTile::Top_Upper | MapTile::Top_Left | MapTile::Top_Right))
				onBottomSection = false;
		}
	}

#if MARK_SURFACE_TYPES
	renderSurfaceTypes();
#endif
}



void Map::populateData(TextureManager* tm, VectorF offset)
{
	populateTileRects(offset);
	populateTileRenderInfo(tm);
	populateTileCollisionInfo();
}


void Map::clearData()
{
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			MapTile& tile = mData[Index(x, y)];

			tile.setTexture(nullptr);
			tile.setRenderType(MapTile::Wall);
			tile.setCollisionType(MapTile::Wall);
			tile.setRect(RectF());
		}
	}
}



void Map::populateTileRects(VectorF offset)
{
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			VectorF position = VectorF(x * mTileSize.x, y * mTileSize.y);
			RectF rect(position + offset, mTileSize);
			mData[y][x].setRect(rect);

#if _DEBUG
			mData[y][x].index = Index(x, y);
#endif
		}
	}
}


void Map::populateTileCollisionInfo()
{
	for (unsigned int x = 0; x < xCount(); x++)
	{
		for (unsigned int y = 0; y < yCount(); y++)
		{
			MapTile& tile = mData[Index(x, y)];
			
			if (tile.isRenderType(MapTile::Floor))
			{
				tile.setCollisionType(MapTile::Floor);
			}
			else
			{
				tile.setCollisionType(MapTile::Wall);
			}

			// In top down view these tiles can be moved 'under'
			if (tile.hasRenderType(MapTile::Top_Left | MapTile::Top_Upper | MapTile::Top_Right))
			{
				tile.setCollisionType(MapTile::Floor);
			}
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


/// --- Population Functions --- ///

void Map::populateTileRenderInfo(TextureManager* tm)
{
	// Label all bottom and top side walls
	topBottom();

	// Add right and left labels
	leftRight();

	// Add corners & Top and bottom segments
	corners();

	// Add corner points
	pointCorners();

	// Remove unused labels
	cleanLabels();

	setTextures(tm);
}


void Map::topBottom()
{
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			Index index(x, y);
			MapTile& tile = mData[index];

			if (tile.hasRenderType(MapTile::Wall))
			{
				// Botton walls
				Index up = index + Index(0, -1);
				Index down = index + Index(0, +1);

				// Bottom type walls
				if (isValidIndex(down) && mData[down].hasRenderType(MapTile::Floor))
				{
					// Tile above
					if (isValidIndex(up))
						mData[up].addRenderType(MapTile::Bottom_Upper);

					mData[index].addRenderType(MapTile::Bottom_Lower);
					continue;
				}

				// Top type walls
				if (isValidIndex(up) && mData[up].hasRenderType(MapTile::Floor))
				{
					// Tile above
					if (isValidIndex(down))
						mData[down].addRenderType(MapTile::Top_Lower);

					mData[index].addRenderType(MapTile::Top_Upper);
					continue;
				}
			}
		}
	}
}
void Map::leftRight()
{
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			Index index(x, y);
			MapTile& tile = mData[index];

			Index left = index + Index(-1, 0);
			Index right = index + Index(+1, 0);

			// Map Top half
			if (tile.isRenderType(MapTile::Wall))
			{
				// Map top half
				if (isValidIndex(right) && mData[right].hasRenderType(MapTile::Floor | MapTile::Bottom_Lower | MapTile::Bottom_Upper))
				{
					mData[index].addRenderType(MapTile::Right);
					continue;
				}

				// Unlike the right side as we are moving right we also skip left walls (as we add them)
				if (isValidIndex(left) && mData[left].hasRenderType(MapTile::Floor | MapTile::Bottom_Lower | MapTile::Bottom_Upper) && !mData[left].hasRenderType(MapTile::Left))
				{
					mData[index].addRenderType(MapTile::Left);
					continue;
				}
			}

			// Map bot half
			if (tile.hasRenderType(MapTile::Wall) && tile.hasRenderType(MapTile::Top_Lower | MapTile::Top_Upper))
			{
				if (isValidIndex(right) && mData[right].hasRenderType(MapTile::Floor))
				{
					mData[index].addRenderType(MapTile::Right);
					continue;
				}

				if (isValidIndex(left) && mData[left].hasRenderType(MapTile::Floor))
				{
					mData[index].addRenderType(MapTile::Left);
					continue;
				}
			}
		}
	}
}
void Map::corners()
{
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			Index index(x, y);
			MapTile& tile = mData[index];

			Index below = index + Index(0, +1);
			Index above = index + Index(0, -1);

			// Map top half
			if (isValidIndex(below) && mData[below].hasRenderType(MapTile::Bottom_Upper))
			{
				// Corners
				if (mData[index].hasRenderType(MapTile::Left))
				{
					mData[index].setRenderType(MapTile::Wall | MapTile::Bottom_Left);
					continue;
				}
				if (mData[index].hasRenderType(MapTile::Right))
				{
					mData[index].setRenderType(MapTile::Wall | MapTile::Bottom_Right);
					continue;
				}
				// Bottom wall Tops
				if (mData[index].isRenderType(MapTile::Wall))
				{
					mData[index].setRenderType(MapTile::Wall | MapTile::Bottom);
					continue;
				}
			}

			// Map bot half
			if (isValidIndex(above))
			{
				// Corners
				if (mData[index].hasRenderType(MapTile::Left))
				{
					if (mData[index].hasRenderType(MapTile::Top_Upper))
					{
						mData[index].setRenderType(MapTile::Wall | MapTile::Top_Left);
						continue;
					}
				}
				else if (mData[index].hasRenderType(MapTile::Right))
				{
					if (mData[index].hasRenderType(MapTile::Top_Upper))
					{
						mData[index].setRenderType(MapTile::Wall | MapTile::Top_Right);
						continue;
					}
				}
			}

		}
	}
}
void Map::pointCorners()
{
	// points
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			Index index(x, y);
			MapTile& tile = mData[index];

			Index down = index + Index(0, +1);
			Index left = index + Index(-1, 0);
			Index right = index + Index(+1, 0);


			// Map top half
			if (mData[index].isRenderType(MapTile::Wall) && isValidIndex(down))
			{
				if (isValidIndex(left) &&
					(mData[left].hasRenderType(MapTile::Bottom | MapTile::Bottom_Left)) &&
					(mData[down].hasRenderType(MapTile::Left | MapTile::Bottom_Left)))
				{
					mData[index].addRenderType(MapTile::Point_Bottom_Left);
				}

				if (isValidIndex(right) &&
					(mData[right].hasRenderType(MapTile::Bottom | MapTile::Bottom_Right)) &&
					(mData[down].hasRenderType(MapTile::Right | MapTile::Bottom_Right)))
				{
					mData[index].addRenderType(MapTile::Point_Bottom_Right);
				}
			}

			// Map bot half
			if (mData[index].hasRenderType(MapTile::Top_Upper | MapTile::Top_Left))
			{
				if (isValidIndex(left))
				{
					if (mData[left].isRenderType(MapTile::Wall) || mData[left].hasRenderType(MapTile::Top_Lower))
					{
						mData[left].addRenderType(MapTile::Point_Top_Right);
					}
				}

				if (isValidIndex(right))
				{
					if (mData[right].isRenderType(MapTile::Wall) || mData[right].hasRenderType(MapTile::Top_Lower))
					{
						mData[right].addRenderType(MapTile::Point_Top_Left);
					}
				}
			}
		}
	}
}
// Once all the tile types have been set top lower = wall
void Map::cleanLabels()
{
	// points
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			Index index(x, y);
			MapTile& tile = mData[index];

			tile.removeRenderType(MapTile::Top_Lower);
		}
	}
}

void Map::setTextures(TextureManager* tm)
{
	std::unordered_map<MapTile::Type, Texture*> tileTextures;
	tileTextures[MapTile::Floor] = tm->getTexture("floor", FileManager::Image_Maps);
	tileTextures[MapTile::Wall] = tm->getTexture("wall", FileManager::Image_Maps);
	tileTextures[MapTile::Left] = tm->getTexture("wall_left", FileManager::Image_Maps);
	tileTextures[MapTile::Right] = tm->getTexture("wall_right", FileManager::Image_Maps);
	tileTextures[MapTile::Bottom_Lower] = tm->getTexture("wall_bottom_lower", FileManager::Image_Maps);
	tileTextures[MapTile::Bottom_Upper] = tm->getTexture("wall_bottom_upper", FileManager::Image_Maps);
	tileTextures[MapTile::Top_Lower] = tm->getTexture("wall_top_lower", FileManager::Image_Maps);
	tileTextures[MapTile::Top_Upper] = tm->getTexture("wall_top_upper", FileManager::Image_Maps);
	tileTextures[MapTile::Bottom] = tm->getTexture("wall_bottom", FileManager::Image_Maps);
	tileTextures[MapTile::Top] = tm->getTexture("wall_top_upper", FileManager::Image_Maps);
	tileTextures[MapTile::Top_Right] = tm->getTexture("wall_top_right", FileManager::Image_Maps);
	tileTextures[MapTile::Top_Left] = tm->getTexture("wall_top_left", FileManager::Image_Maps);
	tileTextures[MapTile::Bottom_Right] = tm->getTexture("wall_bottom_right", FileManager::Image_Maps);
	tileTextures[MapTile::Bottom_Left] = tm->getTexture("wall_bottom_left", FileManager::Image_Maps);
	tileTextures[MapTile::Point_Bottom_Right] = tm->getTexture("point_bottom_right", FileManager::Image_Maps);
	tileTextures[MapTile::Point_Bottom_Left] = tm->getTexture("point_bottom_left", FileManager::Image_Maps);
	tileTextures[MapTile::Point_Top_Right] = tm->getTexture("point_top_right", FileManager::Image_Maps);
	tileTextures[MapTile::Point_Top_Left] = tm->getTexture("point_top_left", FileManager::Image_Maps);

	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			MapTile& tile = mData[Index(x, y)];

			MapTile::Type renderType = tile.renderType();

			if (renderType == MapTile::Wall || renderType == MapTile::Floor)
			{
				tile.setTexture(tileTextures[renderType]);
			}
			else
			{
				// Remove wall and floor components
				renderType &= ~(MapTile::Wall | MapTile::Floor);
				tile.setTexture(tileTextures[renderType]);
			}
		}
	}
}


// Debugging
#if MARK_SURFACE_TYPES
void Map::renderSurfaceTypes()
{
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
				debugDrawRectOutline(Camera::Get()->toCameraCoords(tileRect), RenderColour::Red);

				if (tile.hasRenderType(MapTile::Wall))
				{
					debugRenderText("Wall", fontSize, tileRect.TopCenter());
					tileRect = tileRect.Translate(offset);
				}

				// Floor
				if (tile.hasRenderType(MapTile::Floor))
				{
					debugRenderText("Floor", fontSize, tileRect.TopCenter());
					tileRect = tileRect.Translate(offset);
				}

				// Bottom walls
				if (tile.hasRenderType(MapTile::Bottom_Lower))
				{
					debugRenderText("Bottom lower", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				if (tile.hasRenderType(MapTile::Bottom_Upper))
				{
					debugRenderText("Bottom upper", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				// Top walls
				if (tile.hasRenderType(MapTile::Top_Lower))
				{
					debugRenderText("Top lower", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				if (tile.hasRenderType(MapTile::Top_Upper))
				{
					debugRenderText("Top upper", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				// Side walls
				if (tile.hasRenderType(MapTile::Right))
				{
					debugRenderText("Right", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				if (tile.hasRenderType(MapTile::Left))
				{
					debugRenderText("Left", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				if (tile.hasRenderType(MapTile::Bottom))
				{
					debugRenderText("Bottom", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				// Corners
				if (tile.hasRenderType(MapTile::Bottom_Right))
				{
					debugRenderText("Bottom right", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				if (tile.hasRenderType(MapTile::Bottom_Left))
				{
					debugRenderText("Bottom left", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				if (tile.hasRenderType(MapTile::Top_Right))
				{
					debugRenderText("Top right", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				if (tile.hasRenderType(MapTile::Top_Left))
				{
					debugRenderText("Top left", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				// Points
				if (tile.hasRenderType(MapTile::Point_Bottom_Right))
				{
					debugRenderText("bot right point", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				if (tile.hasRenderType(MapTile::Point_Bottom_Left))
				{
					debugRenderText("bot left point", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				if (tile.hasRenderType(MapTile::Point_Top_Right))
				{
					debugRenderText("top right point", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}

				if (tile.hasRenderType(MapTile::Point_Top_Left))
				{
					debugRenderText("top left point", fontSize, tileRect.Center());
					tileRect = tileRect.Translate(offset);
				}
			}
		}
	}
}
#endif 

