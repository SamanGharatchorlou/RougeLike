#pragma once


class AIPathMap;
class PathTile;

using TileCost = std::pair<const PathTile*, int>;
using Index = Vector2D<int>;
using Path = std::stack<Index>;

class AIPathing
{
public:
	AIPathing() : mMap(nullptr) { }
	AIPathing(const AIPathMap* map);

	void init(const AIPathMap* map) { mMap = map; }
	void clear() { mMap = nullptr; }

	Path findPath(VectorF start, VectorF end, int pathLimit) const;

	Index index(VectorF position) const;
	VectorF position(Index tileIndex) const;

	const PathTile* tile(Index index) const;

	const AIPathMap* map() const { return mMap; }

	bool updateCurrentIndex(VectorF position);


private:
	Path getPath(Index start, Index end, Grid<Index>& pathing) const;

	Index nearestFloorTile(Index index) const;

	inline int heuristic(Index from, Index to) const;
	
	struct GreaterThanByCost
	{
		bool operator ()(TileCost left, TileCost right)
		{
			return left.second > right.second;
		};
	};


private:
	const AIPathMap* mMap;

	Index mCurrentIndex;
};


inline int AIPathing::heuristic(Index from, Index to) const
{
	return std::abs(from.x - to.x) + std::abs(from.y - to.y);
}