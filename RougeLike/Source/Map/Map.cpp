#include "pch.h"
#include"Map/Map.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"

#include "TunnelGenerator.h"


void Map::init(Vector2D<int> size)
{
	mData.set(size, MapTile(MapTile::Wall));
}


void Map::populateData(VectorF offset)
{
	populateTileRects(offset);
	populateTileRenderInfo();
	
	//populateCollisionRenderInfo();
}

void Map::render(const TextureManager* tm)
{
	Texture* floor = tm->getTexture("floor", FileManager::Image_Maps); 
	Texture* wall = tm->getTexture("wall", FileManager::Image_Maps);

	Texture* wall_BL = tm->getTexture("wall_bottom_lower", FileManager::Image_Maps);
	Texture* wall_BU = tm->getTexture("wall_bottom_upper", FileManager::Image_Maps);
	Texture* wall_TL = tm->getTexture("wall_top_lower", FileManager::Image_Maps);
	Texture* wall_TU = tm->getTexture("wall_top_upper", FileManager::Image_Maps);

	Texture* wall_Right = tm->getTexture("wall_right", FileManager::Image_Maps);
	Texture* wall_Left = tm->getTexture("wall_left", FileManager::Image_Maps);
	Texture* wall_bottom = tm->getTexture("wall_bottom", FileManager::Image_Maps);

	Texture* wall_TopRight = tm->getTexture("wall_top_right", FileManager::Image_Maps);
	Texture* wall_TopLeft = tm->getTexture("wall_top_left", FileManager::Image_Maps);
	Texture* wall_BotRight = tm->getTexture("wall_bottom_right", FileManager::Image_Maps);
	Texture* wall_BotLeft = tm->getTexture("wall_bottom_left", FileManager::Image_Maps);

	Texture* point_bottom_right = tm->getTexture("point_bottom_right", FileManager::Image_Maps);
	Texture* point_bottom_left = tm->getTexture("point_bottom_left", FileManager::Image_Maps);


	Camera* camera = Camera::Get();

	for (unsigned int y = 0; y < yCount(); y++)
	{
		for (unsigned int x = 0; x < xCount(); x++)
		{
			MapTile tile = mData[y][x];
			Rect<int> tileRect = tile.rect();

			if (camera->inView(tileRect))
			{
				tileRect = camera->toCameraCoords(tileRect);

				// Split each floor tile into 4
				if (tile.hasRenderType(MapTile::Floor))
				{
					floor->render(tileRect);

					// Split tile into 4 pieces
					//VectorF size = tileRect.Size() / 2.0f;
					//tileRect.SetSize(size);

					//// Top left
					//floor->render(tileRect);

					//// Top right
					//floor->render(tileRect.Translate(VectorF(tileRect.Size().x, 0.0f)));

					//// Bot right
					//floor->render(tileRect.Translate(VectorF(tileRect.Size().x, tileRect.Size().x)));

					//// Bot left
					//floor->render(tileRect.Translate(VectorF(0.0f, tileRect.Size().x)));

				}

				if (tile.isRenderType(MapTile::Wall))
					wall->render(tileRect);

				if (tile.hasRenderType(MapTile::Top_Right))
					wall_TopRight->render(tileRect);

				if (tile.hasRenderType(MapTile::Top_Left))
					wall_TopLeft->render(tileRect);

				if (tile.hasRenderType(MapTile::Bottom_Right))
					wall_BotRight->render(tileRect);

				if (tile.hasRenderType(MapTile::Bottom_Left))
					wall_BotLeft->render(tileRect);

				if (tile.hasRenderType(MapTile::Bottom_Lower))
					wall_BL->render(tileRect);

				if (tile.hasRenderType(MapTile::Bottom_Upper))
					wall_BU->render(tileRect);

				if (tile.hasRenderType(MapTile::Top_Lower))
					wall_TL->render(tileRect);

				if (tile.hasRenderType(MapTile::Top_Upper))
					wall_TU->render(tileRect);

				if (tile.hasRenderType(MapTile::Right))
					wall_Right->render(tileRect);

				if (tile.hasRenderType(MapTile::Left))
					wall_Left->render(tileRect);

				if (tile.hasRenderType(MapTile::Bottom))
					wall_bottom->render(tileRect);

				if (tile.hasRenderType(MapTile::Point_Bottom_Left))
					point_bottom_left->render(tileRect);

				if (tile.hasRenderType(MapTile::Point_Bottom_Right))
					point_bottom_right->render(tileRect);
			}
		}
	}
#if MARK_SURFACE_TYPES
	renderSurfaceTypes();
#endif
}

#if MARK_SURFACE_TYPES
void Map::renderSurfaceTypes()
{
	Camera* camera = Camera::Get();

	int intView = 0;

	for (unsigned int y = 0; y < yCount(); y++)
	{
		for (unsigned int x = 0; x < xCount(); x++)
		{
			MapTile tile = mData[y][x];
			RectF tileRect = tile.rect();

			if (camera->inView(tileRect))
			{
				intView++;

				float yOffset = 0.0f;

				debugDrawRectOutline(Camera::Get()->toCameraCoords(tileRect), RenderColour::Red);

				if (tile.isRenderType(MapTile::Wall))
				{
					debugRenderText("Wall", 16, tileRect.TopCenter());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				// Floor
				if (tile.hasRenderType(MapTile::Floor))
				{
					debugRenderText("Floor", 16, tileRect.TopCenter());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				// Bottom walls
				if (tile.hasRenderType(MapTile::Bottom_Lower))
				{
					debugRenderText("Bottom lower", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				if (tile.hasRenderType(MapTile::Bottom_Upper))
				{
					debugRenderText("Bottom upper", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				// Top walls
				if (tile.hasRenderType(MapTile::Top_Lower))
				{
					debugRenderText("Top lower", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				if (tile.hasRenderType(MapTile::Top_Upper))
				{
					debugRenderText("Top upper", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				// Side walls
				if (tile.hasRenderType(MapTile::Right))
				{
					debugRenderText("Right", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				if (tile.hasRenderType(MapTile::Left))
				{
					debugRenderText("Left", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				if (tile.hasRenderType(MapTile::Bottom))
				{
					debugRenderText("Bottom", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				// Corners
				if (tile.hasRenderType(MapTile::Bottom_Right))
				{
					debugRenderText("Bottom right", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				if (tile.hasRenderType(MapTile::Bottom_Left))
				{
					debugRenderText("Bottom left", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				if (tile.hasRenderType(MapTile::Top_Right))
				{
					debugRenderText("Bottom right", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				if (tile.hasRenderType(MapTile::Top_Left))
				{
					debugRenderText("Bottom left", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				// Points
				if (tile.hasRenderType(MapTile::Point_Bottom_Right))
				{
					debugRenderText("bot right point", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}

				if (tile.hasRenderType(MapTile::Point_Bottom_Left))
				{
					debugRenderText("bot left point", 16, tileRect.Center());
					tileRect = tileRect.Translate(VectorF(0.0f, 20.0f));
				}
			}
		}
	}
}
#endif


void Map::renderBottomLayer(const TextureManager* tm, float yPoint)
{
//#if _DEBUG
//	tileRenderCounter = 0;
//#endif
//
//	Texture* floor = tm->getTexture("floor", FileManager::Image_Maps);
//	Texture* wall = tm->getTexture("wall", FileManager::Image_Maps);
//
//	Texture* leftEdge = tm->getTexture("wall_left_edge", FileManager::Image_Maps);
//	Texture* rightEdge = tm->getTexture("wall_right_edge", FileManager::Image_Maps);
//	Texture* botEdge = tm->getTexture("wall_bot_edge", FileManager::Image_Maps);
//
//	Texture* column = tm->getTexture("columnsmall", FileManager::Image_Maps);
//
//	Camera* camera = Camera::Get();
//
//	for (unsigned int y = 0; y < yCount(); y++)
//	{
//		for (unsigned int x = 0; x < xCount(); x++)
//		{
//			MapTile tile = mData[y][x];
//			Rect<int> tileRect = tile.rect();
//
//			if (camera->inView(tileRect))
//			{
//				// Render walls 'below/under' the player after the player 
//				if (tileRect.Center().y >= yPoint && !tile.isRenderType(MapTile::Floor))
//					continue;
//
//				tileRect = camera->toCameraCoords(tileRect);
//
//				if(tile.hasRenderType(MapTile::Floor))
//					floor->render(tileRect);
//
//				if (tile.hasRenderType(MapTile::ColumnBot))
//					renderColumn(tileRect, column);
//
//				if (tile.hasRenderType(MapTile::Wall))
//					wall->render(tileRect);
//
//				if (tile.hasRenderType(MapTile::Left))
//					leftEdge->render(tileRect);
//
//				if (tile.hasRenderType(MapTile::Right))
//					rightEdge->render(tileRect);
//
//				if (tile.hasRenderType(MapTile::Bot))
//					botEdge->render(tileRect, SDL_FLIP_VERTICAL);
//
//				if (tile.hasRenderType(MapTile::Top))
//					botEdge->render(tileRect, SDL_FLIP_VERTICAL);
//
//				if (tile.hasRenderType(MapTile::ColumnTop))
//					renderColumn(tileRect, column);
//
//#if _DEBUG
//				tileRenderCounter++;
//#endif
//			}
//		}
//	}
}


void Map::renderTopLayer(const TextureManager* tm, float yPoint)
{
//	Texture* floor = tm->getTexture("floor", FileManager::Image_Maps);
//	Texture* wall = tm->getTexture("wall", FileManager::Image_Maps);
//
//	Texture* leftEdge = tm->getTexture("wall_left_edge", FileManager::Image_Maps);
//	Texture* rightEdge = tm->getTexture("wall_right_edge", FileManager::Image_Maps);
//	Texture* botEdge = tm->getTexture("wall_bot_edge", FileManager::Image_Maps);
//
//	Texture* column = tm->getTexture("columnsmall", FileManager::Image_Maps);
//
//	Camera* camera = Camera::Get();
//
//	for (unsigned int y = 0; y < yCount(); y++)
//	{
//		for (unsigned int x = 0; x < xCount(); x++)
//		{
//			MapTile tile = mData[y][x];
//			Rect<int> tileRect = tile.rect();
//
//			if (camera->inView(tileRect))
//			{
//				MapTile tile = mData[y][x];
//
//				// skip anything that would have been rendered in layer A
//				if (tile.rect().Center().y < yPoint || tile.isRenderType(MapTile::Floor))
//					continue;
//
//				tileRect = camera->toCameraCoords(tileRect);
//
//				if (tile.hasRenderType(MapTile::Floor))
//					floor->render(tileRect);
//
//				if (tile.hasRenderType(MapTile::ColumnBot))
//					renderColumn(tileRect, column);
//
//				if (tile.hasRenderType(MapTile::Wall))
//					wall->render(tileRect);
//
//				if (tile.hasRenderType(MapTile::Left))
//					leftEdge->render(tileRect);
//
//				if (tile.hasRenderType(MapTile::Right))
//					rightEdge->render(tileRect);
//
//				if (tile.hasRenderType(MapTile::Bot))
//					botEdge->render(tileRect, SDL_FLIP_VERTICAL);
//
//				if (tile.hasRenderType(MapTile::Top))
//					botEdge->render(tileRect, SDL_FLIP_VERTICAL);
//
//				if (tile.hasRenderType(MapTile::ColumnTop))
//					renderColumn(tileRect, column);
//
//#if _DEBUG
//				tileRenderCounter++;
//#endif
//			}
//		}
//	}
//
//#if _DEBUG
//	float tilesInCamera = (Camera::Get()->size() / tileSize()).area();
//
//	if (tileRenderCounter > tilesInCamera + (yCount() * 2) + (xCount() * 2))
//		DebugPrint(Log, "There are approx %f tiles within the viewport and we are rendering %f, too many?\n",
//			tilesInCamera + (yCount() * 2) + (xCount() * 2), tileRenderCounter);
//#endif
}


void Map::renderColumn(const RectF& rect, Texture* column)
{
	//VectorF size = column->originalDimentions * 3.0f;
	//VectorF position = rect.BotRight();

	//RectF columnRect = RectF(position, size);
	//columnRect.SetBotRight(position);

	//column->render(columnRect);
}



void Map::populateTileRects(VectorF offset)
{
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			VectorF size(128.0f, 128.0f); // TODO: hard coded
			VectorF position = VectorF(x * size.x, y * size.y);
			RectF rect(position + offset, size);

			mData[y][x].setRect(rect);

#if _DEBUG
			mData[y][x].index = Index(x, y);
#endif
		}
	}
}


void Map::populateTileRenderInfo()
{
	// First pass - Label all bottom and top side walls
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
				Index left = index + Index(-1, 0);
				Index right = index + Index(+1, 0);

				// Bottom type walls
				if (isValidIndex(down) && mData[down].hasRenderType(MapTile::Floor))
				{
					// Tile above
					if (isValidIndex(up))
						mData[up].addRenderType(MapTile::Bottom_Upper);

					// Current tile
					mData[index].addRenderType(MapTile::Bottom_Lower);

					// bottom lower tiles cant have any other types
					//continue;
				}

				//// Top type walls
				//if (isValidIndex(down) && mData[down].hasRenderType(MapTile::Floor))
				//{
				//	// Tile below
				//	if (isValidIndex(up))
				//		mData[up].addRenderType(MapTile::Top_Lower);

				//	// Current tile
				//	mData[index].addRenderType(MapTile::Top_Upper);
				//}
			}
		}
	}

	// Add right and left labels
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			Index index(x, y);
			MapTile& tile = mData[index];

			if (tile.isRenderType(MapTile::Wall))
			{
				// Botton walls
				Index up = index + Index(0, 1);
				Index down = index + Index(0, -1);
				Index left = index + Index(-1, 0);
				Index right = index + Index(+1, 0);

				// Right walls
				if (isValidIndex(right) && !mData[right].isRenderType(MapTile::Wall))
				{
					// Current tile
					mData[index].addRenderType(MapTile::Right);
				}

				// Left walls
				// Unlike the right side as we are moving right we also skip left walls (as we add them)
				if (isValidIndex(left) && !mData[left].isRenderType(MapTile::Wall) && !mData[left].hasRenderType(MapTile::Left))
				{
					// Current tile
					mData[index].addRenderType(MapTile::Left);
				}
			}
		}
	}


	// Add corner & other labels
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			Index index(x, y);
			MapTile& tile = mData[index];

			Index below = index + Index(0, +1);
			if (isValidIndex(below))
			{
				// Corners
				if (mData[index].hasRenderType(MapTile::Left))
				{
					if (mData[below].hasRenderType(MapTile::Bottom_Upper))
					{
						// bottom U
						mData[index].setRenderType(MapTile::Wall);
						mData[index].addRenderType(MapTile::Bottom_Left);
					}
				}
				else if (mData[index].hasRenderType(MapTile::Right))
				{
					if (mData[below].hasRenderType(MapTile::Bottom_Upper))
					{
						// bottom U
						mData[index].setRenderType(MapTile::Wall);
						mData[index].addRenderType(MapTile::Bottom_Right);
					}
				}
				// Tops
				else if (mData[index].isRenderType(MapTile::Wall))
				{
					if (mData[below].hasRenderType(MapTile::Bottom_Upper))
					{
						// bottom U
						mData[index].setRenderType(MapTile::Wall);
						mData[index].addRenderType(MapTile::Bottom);
					}
				}
			}
		}
	}

	// points
	for (int y = 0; y < mData.yCount(); y++)
	{
		for (int x = 0; x < mData.xCount(); x++)
		{
			Index index(x, y);
			MapTile& tile = mData[index];

			Index up = index + Index(0, -1);
			Index down = index + Index(0, +1);
			Index left = index + Index(-1, 0);
			Index right = index + Index(+1, 0);

			if (isValidIndex(left) && isValidIndex(down))
			{
				if (mData[index].isRenderType(MapTile::Wall))
				{
					if (mData[left].hasRenderType(MapTile::Bottom) && 
						(mData[down].hasRenderType(MapTile::Left) || mData[down].hasRenderType(MapTile::Bottom_Left)))
					{
						// bottom U
						mData[index].setRenderType(MapTile::Wall);
						mData[index].addRenderType(MapTile::Point_Bottom_Left);
					}
				}
			}

			if (isValidIndex(right) && isValidIndex(down))
			{
				if (mData[index].isRenderType(MapTile::Wall))
				{
					if (mData[right].hasRenderType(MapTile::Bottom) && 
						(mData[down].hasRenderType(MapTile::Right) || mData[down].hasRenderType(MapTile::Bottom_Right)))
					{
						// bottom U
						mData[index].setRenderType(MapTile::Wall);
						mData[index].addRenderType(MapTile::Point_Bottom_Right);
					}
				}
			}
		}
	}
}


void Map::populateCollisionRenderInfo()
{
//	for (unsigned int x = 0; x < xCount(); x++)
//	{
//		bool floorAboveReached = false;
//
//		for (unsigned int y = 0; y < yCount(); y++)
//		{
//			// query surronding tiles
//			Index index(x, y);
//			const MapTile::EdgeInfo& info = getEdgeInfo(index);
//			MapTile& tile = mData[index];
//
//			if (info.hasEdge)
//			{
//				// floor left
//				if (info.data[1][0] == MapTile::Floor)
//				{
//					tile.addRenderType(MapTile::Left);
//					tile.addCollisionType(MapTile::Left);
//				}
//
//				// floor right
//				if (info.data[1][2] == MapTile::Floor)
//				{
//					tile.addRenderType(MapTile::Right);
//					tile.addCollisionType(MapTile::Right);
//				}
//
//
//				// Only left and right needs to be considered once true
//				if (floorAboveReached)
//				{
//					tile.removeCollisionType(MapTile::Floor);
//					tile.addCollisionType(MapTile::Wall);
//					continue;
//				}
//
//				// floor above
//				if (info.data[0][1] == MapTile::Floor)
//				{
//					// Editing the tile below messes with the looping
//					// only edit the sides after this has been hit
//					floorAboveReached = true;
//
//					tile.addRenderType(MapTile::Top);
//
//					Index rightIndex(x, y + 1);
//					if (inBounds(rightIndex) && wallRenderTile(rightIndex))
//					{
//						MapTile::Type tileType = tile.collisionType();
//
//						mData[rightIndex].setCollisionType(tileType ^ MapTile::Top);
//
//						int a = 0;
//					}
//
//					tile.setCollisionType(MapTile::Floor);
//				}
//
//				// floor below
//				if (info.data[2][1] == MapTile::Floor)
//				{
//					tile.addRenderType(MapTile::Bot);
//					tile.addCollisionType(MapTile::Bot);
//
//					tile.removeRenderType(MapTile::Left ^ MapTile::Right);
//
//					// Add isometic wall edges if the left/right tile is a wall but is not a MapTile::Bot
//					Index leftIndex(x - 1, y);
//					if (inBounds(leftIndex) && wallRenderTile(leftIndex) && !(mData[leftIndex].hasRenderType(MapTile::Bot)))
//					{
//						mData[leftIndex].addRenderType(MapTile::Right);
//					}
//
//					Index rightIndex(x + 1, y);
//					if (inBounds(rightIndex) && wallRenderTile(rightIndex) && !(mData[rightIndex].hasRenderType(MapTile::Bot)))
//					{
//						mData[rightIndex].addRenderType(MapTile::Left);
//					}
//				}
//			}
//		}
//	}
//
//#if DRAW_BINARY_MAP
//	printBinaryMap();
//#endif
}


const MapTile* Map::offsetTile(const MapTile* target, int xOffset, int yOffset) const
{
	Index tileIndex = index(target) + Index(xOffset, yOffset);
	return isValidIndex(tileIndex) ? tile(tileIndex) : nullptr;
}


Index Map::findYFloorTileRange(int xTileIndex) const
{
	unsigned int yTileIndex = 0;
	Vector2D<unsigned int> yTileRange;

	while (wallCollisionTile(Index(xTileIndex, ++yTileIndex))) {}

	// highest point
	yTileRange.x = yTileIndex;

	while (floorCollisionTile(Index(xTileIndex, ++yTileIndex)))
	{
		if (yTileIndex >= yCount() - 1)
			break;
	}

	// remove last increment to keep within floor and minus one extra to prevent enemy moving behind the wall
	yTileRange.y = clamp(yTileIndex - 2, yTileRange.x, yCount() - 2);

	return yTileRange;
}


// --- Getters --- //
const MapTile::EdgeInfo Map::getEdgeInfo(Index index) const
{
	MapTile::EdgeInfo info;

	// Is a wall tile
	if (wallRenderTile(index))
	{
		for (int j = index.y - 1; j <= index.y + 1; j++)
		{
			for (int i = index.x - 1; i <= index.x + 1; i++)
			{
				// If touching a floor tile
				Index tileIndex(i, j);
				if (inBounds(tileIndex) && floorRenderTile(tileIndex))
				{
					Index infoIndex = tileIndex - index + 1;
					info.data[infoIndex.y][infoIndex.x] = MapTile::Floor;
					info.hasEdge = true;
				}
			}
		}
	}

	return info;
}


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


const RectF Map::tileRect(Index index) const
{
	return isValidIndex(index) ? mData.get(index).rect() : RectF(-1);
}


const RectF Map::getFirstRect(int yIndex) const
{
	return tileRect(Index(0, yIndex));
}

const RectF Map::getLastRect(int yIndex) const
{
	return tileRect(Index(xCount() - 1, yIndex));
}


void Map::addTileType(Index index, MapTile::Type type)
{
	mData[index].addRenderType(type);

	if (type >= MapTile::Wall)
		mData[index].setCollisionType(MapTile::Wall);
	else
		mData[index].setCollisionType(MapTile::Floor);
}


void Map::setTileType(Index index, MapTile::Type type)
{
	mData[index].setType(type);
}


void Map::removeTileType(Index index, MapTile::Type type)
{
	mData[index].addRenderType(type);

	if (type >= MapTile::Wall)
		mData[index].setCollisionType(MapTile::Wall);
	else
		mData[index].setCollisionType(MapTile::Floor);
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