#pragma once

#include "Events/Dispatcher.h"

class AIPathMap;
class Enemy;
class Map;

using EnemyList = std::vector<Enemy*>;

class AIPathingController : public Dispatcher
{
public:
	AIPathingController();

	void clear();

	void addMap(const Map* map);
	AIPathMap* popMap();

	void clearCostMaps();

	void updatePaths(const EnemyList& enemies, float dt);

	void updateAIPathCostMap(const EnemyList& enemies);

	AIPathMap* pathMap(const Map* map);
	AIPathMap* findPathMap(const AIPathMap* pathMap);

	int mapCount() const { return mPathMaps.size(); }

#if DRAW_AI_PATH_COSTMAP
	void drawCostMap();
#endif


private:
	int calculatePathingLimit(int minimunFrameCount, float dt);
	Vector2D<int> getCalculationIndexRange(const EnemyList& enemies);


private:
	std::vector<AIPathMap*> mPathMaps;
	
	Vector2D<int> mCalculationIndexRange;

	// A* algorithm only builds a path upto this limit
	int mPathLimit;

	// Split the enemy pathing calculations across frames
	int mListSplit;
	int mSegmentLength;
	int mSegmentIndex;
};

