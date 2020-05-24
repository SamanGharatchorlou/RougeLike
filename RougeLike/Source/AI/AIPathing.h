#pragma once


class AIPathMap;
class PathTile;

using TileCost = std::pair<const PathTile*, int>;
using Index = Vector2D<int>;
using Path = std::stack<Index>;

class AIPathing
{
public:
	AIPathing(AIPathMap* map);

	Path findPath(VectorF start, VectorF end);

	Index index(VectorF position) const;
	VectorF position(Index tileIndex) const;

	// TEMP
	PathTile tile(Index index);

#if DRAW_AI_PATH
	std::vector<PathTile> debugPath;
	void draw();
#endif

private:
	Path getPath(Index start, Index end, Grid<Index>& pathing);

	Index nearestFloorTile(Index index) const;

	inline int heuristic(Index from, Index to);
	
	struct GreaterThanByCost
	{
		bool operator ()(TileCost left, TileCost right)
		{
			return left.second > right.second;
		};
	};


private:
	AIPathMap* mMap;
};


inline int AIPathing::heuristic(Index from, Index to)
{
	return std::abs(from.x - to.x) + std::abs(from.y - to.y);
}