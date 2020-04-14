#pragma once


class Map;
class MapTile;

using TileCost = std::pair<const MapTile*, int>;
using Path = std::stack<Vector2D<int>>;

class AIPathing
{
public:
	AIPathing(Map* map);

	Path findPath(VectorF start, VectorF end);

	Vector2D<int> getTileIndex(VectorF position) const;
	VectorF getTilePosition(Vector2D<int> tileIndex) const;



private:
	Path getPath(Vector2D<int> start, Vector2D<int> end, Grid<Vector2D<int>>& pathing);

	Vector2D<int> nearestFloorTile(Vector2D<int> index) const;

	inline int heuristic(Vector2D<int> from, Vector2D<int> to);
	
	struct GreaterThanByCost
	{
		bool operator ()(TileCost left, TileCost right)
		{
			return left.second > right.second;
		};
	};


private:
	Map* mMap;
};


inline int AIPathing::heuristic(Vector2D<int> from, Vector2D<int> to)
{
	return std::abs(from.x - to.x) + std::abs(from.y - to.y);
}