#include "pch.h"
#include "AIPathing.h"

#include "AIPathMap.h"


AIPathing::AIPathing(AIPathMap* map) : mMap(map) { }


CostMap* AIPathing::costMap() { return &mMap->costMapRef(); }

std::stack<Index> AIPathing::findPath(VectorF startPosition, VectorF endPosition) const
{
	Index startingIndex = mMap->index(startPosition);
	Index endIndex = mMap->index(endPosition);

	if (!mMap->isValidIndex(endIndex) || 
		startingIndex == endIndex || 
		!mMap->floorCollisionTile(endIndex))
		return Path();

	// Lowest to highest path cost queue
	std::priority_queue<TileCost, std::vector<TileCost>, GreaterThanByCost> frontier;
	frontier.push(TileCost(tile(startingIndex), 0));

	Grid<Index> cameFrom(mMap->yCount(), mMap->xCount(), Index(-1,-1));
	Grid<int> cost(mMap->yCount(), mMap->xCount(), 0);
	const Grid<int>& globalCostMap = mMap->costMap();


#if _DEBUG
	int loopCounter = 0;
#endif

	while (!frontier.empty())
	{
		const PathTile* currentTile = frontier.top().first;
		const PathTile* neighbours[4] = {
			mMap->offsetTile(currentTile, 0, -1),
			mMap->offsetTile(currentTile, 1, 0),
			mMap->offsetTile(currentTile, 0, 1),
			mMap->offsetTile(currentTile, -1, 0) };

		// Search all neighbours
		for (unsigned int i = 0; i < 4; i++)
		{
			const PathTile* nextTile = *(neighbours + i);

			// valid tile
			if (nextTile && nextTile->is(CollisionTile::Floor))
			{
				Index nextIndex = mMap->index(nextTile);
				Index currentIndex = mMap->index(currentTile);

				int newCost = cost[currentIndex] + globalCostMap.get(nextIndex); // replace 1 with the floor type

				if (mMap->inBounds(nextIndex) &&
					(cameFrom.get(nextIndex).isNegative() || newCost < cost[nextIndex]))
				{
					int priority = newCost + heuristic(endIndex, nextIndex);
					frontier.push(TileCost(nextTile, priority));

					cameFrom[nextIndex] = currentIndex;
					cost[nextIndex] = newCost;

					if (nextIndex == endIndex)
					{
						return getPath(startingIndex, endIndex, cameFrom);
					}
				}
			}
		}

		frontier.pop();

#if _DEBUG
		if (frontier.size() > mMap->yCount() * mMap->xCount())
		{
			DebugPrint(Log,
				"Whole map searched: No valid path was found from index %d,%d to index %d,%d\n",
				startingIndex.x, startingIndex.y, endIndex.x, endIndex.y);
			return Path();
		}

		loopCounter++;
#endif
	}

#if _DEBUG
	int largeNumberOfLoops = 50;
	if (loopCounter > largeNumberOfLoops)
	{
		DebugPrint(Log,
			"Empty frontier: No valid path was found from index %d,%d to index %d,%d -- %d loops were made until this failed\n",
			startingIndex.x, startingIndex.y, endIndex.x, endIndex.y, loopCounter);
	}
#endif

	return Path();
}


Path AIPathing::getPath(Index start, Index finish, Grid<Index>& pathing) const
{
	Path path;
	path.push(finish);

	while (pathing[path.top()] != start)
	{
		path.push(pathing[path.top()]);
	}
  
	return path;
}


// For Enemy class
VectorF AIPathing::position(Index tileIndex) const
{
	return tile(tileIndex)->rect().Center();
}


// TEMP
const PathTile* AIPathing::tile(Index tileIndex) const
{
	return &mMap->get(tileIndex);
}


Index AIPathing::index(VectorF position) const
{
	return mMap->index(position);
}


//
//Vector2D<int> AIPathing::yTileFloorRange(Index index) const
//{
//	ASSERT(Warning, tile(index)->is(CollisionTile::Floor), "Not a floor tile, cannot get yTile floor range");
//
//	Index bottomIndex(index);
//	while (true)
//	{
//		Index increment(0, 1);
//		if (!tile(bottomIndex + increment)->is(CollisionTile::Floor))
//			break;
//
//		bottomIndex += increment;
//	}
//
//	Index topIndex(index);
//	while (true)
//	{
//		Index increment(0, -1);
//		if (!tile(topIndex + increment)->is(CollisionTile::Floor))
//			break;
//
//		topIndex += increment;
//	}
//
//	return Vector2D<int>(topIndex.y, bottomIndex.y);
//}





//#if DRAW_AI_PATH
//void AIPathing::draw()
//{
//	if (debugPath.size() > 1)
//	{
//		for (int i = 0; i < debugPath.size() - 1; i++)
//		{
//			RectF rect = debugPath[i].rect();
//
//			VectorF pointA = debugPath[i].rect().Center();
//			VectorF pointB = debugPath[i + 1].rect().Center();
//
//			debugDrawLine(pointA, pointB, RenderColour::Green);
//		}
//	}
//}
//#endif


// --- Private Functions --- //

// Fix when player is in wall, and invalid end path is given
Index AIPathing::nearestFloorTile(Index index) const
{
	const PathTile* wallTile = tile(index);

	const PathTile* neighbours[4] = {
		mMap->offsetTile(wallTile, 0, -1),
		mMap->offsetTile(wallTile, 1, 0),
		mMap->offsetTile(wallTile, 0, 1),
		mMap->offsetTile(wallTile, -1, 0) };

	// Search all neighbours
	for (unsigned int i = 0; i < 4; i++)
	{
		if (neighbours[i] && neighbours[i]->is(CollisionTile::Floor))
		{
			return mMap->index(neighbours[i]);
		}
	}

	DebugPrint(Warning, "No valid tile was found, this means there are no avaliable neighbour tiles...how?\n");
	return index;
}