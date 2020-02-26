#include "pch.h"
#include "AIPathing.h"

#include "Map/Map.h"


AIPathing::AIPathing(Map* map) : mMap(map) { }


std::stack<Vector2D<int>> AIPathing::findPath(VectorF startPosition, VectorF endPosition)
{
	Vector2D<int> startingIndex = mMap->getIndex(startPosition);
	Vector2D<int> endIndex = mMap->getIndex(endPosition);

	// Lowest to highest path cost queue
	std::priority_queue<TileCost, std::vector<TileCost>, GreaterThanByCost> frontier;
	frontier.push(TileCost(mMap->getTile(startingIndex), 0));

	Grid<Vector2D<int>> cameFrom(mMap->yCount(), mMap->xCount(), Vector2D<int>(-1,-1));
	Grid<int> cost(mMap->yCount(), mMap->xCount(), 0);

	// start search coming from nowhere with 0 cost
	cameFrom[startingIndex] = Vector2D<int>(-1, -1);
	cost[startingIndex] = 0;

	while (!frontier.empty())
	{
		const MapTile* currentTile = frontier.top().first;
		const MapTile* neighbours[4] = { 
			mMap->offsetTile(currentTile, 0, -1),
			mMap->offsetTile(currentTile, 1, 0),
			mMap->offsetTile(currentTile, 0, 1),
			mMap->offsetTile(currentTile, -1, 0) };

		// Search all neighbours
		for (unsigned int i = 0; i < 4; i++)
		{
			const MapTile* nextTile = *(neighbours + i);
			if (nextTile->hasCollisionType(MapTile::Floor))
			{
				Vector2D<int> index = mMap->getIndex(nextTile);

				int newCost = cost[mMap->getIndex(currentTile)] + 1; // replace 1 with the floor type

				if (mMap->inBounds(index.x, index.y) && 
					(cameFrom.get(index).isNegative() || newCost < cost[index]))
				{
					int priority = newCost + heuristic(endIndex, index);
					frontier.push(TileCost(nextTile, priority));

					cameFrom[index] = mMap->getIndex(currentTile);
					cost[index] = newCost;

					if (index == endIndex)
					{
						return getPath(startingIndex, endIndex, cameFrom);
					}
				}
			}
		}

		frontier.pop();

		if (frontier.size() > mMap->yCount() * mMap->xCount())
		{
			DebugPrint(Log, 
				"Whole map searched: No valid path was found from index %d,%d to index %d,%d\n",
				startingIndex.x, startingIndex.y, endIndex.x, endIndex.y);
			return std::stack<Vector2D<int>>();
		}
	}

	DebugPrint(Log, 
		"Empty frontier: No valid path was found from index %d,%d to index %d,%d\n",
		startingIndex.x, startingIndex.y, endIndex.x, endIndex.y);

	return Path();
}


Path AIPathing::getPath(Vector2D<int> start, Vector2D<int> finish, Grid<Vector2D<int>>& pathing)
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
VectorF AIPathing::getTilePosition(Vector2D<int> tileIndex) const
{
	return mMap->getTileRect(tileIndex).Center();
}


Vector2D<int> AIPathing::getTileIndex(VectorF position) const
{
	return mMap->getIndex(position);
}