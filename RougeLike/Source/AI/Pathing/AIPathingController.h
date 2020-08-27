#pragma once

#include "Events/Dispatcher.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif

class AIPathMap;
class Enemy;
class Map;

class AIPathingController : public Dispatcher
{
public:
	AIPathingController();

	void clear();

	void addMap(const Map* map);
	AIPathMap* popMap();

	void clearCostMaps();

	void updatePaths(std::vector<Enemy*> enemies);
	void updateAIPathCostMap(std::vector<Enemy*> enemies);
	void addPathUpdateRequest() { pathUpdateRequests++; }

	AIPathMap* pathMap(const Map* map);
	AIPathMap* findPathMap(const AIPathMap* pathMap);

	int mapCount() const { return mPathMaps.size(); }

#if DRAW_AI_PATH_COSTMAP
	void drawCostMap();
#endif


private:
	void recalculateEnemyPaths(std::vector<Enemy*> enemies);

private:
	std::vector<AIPathMap*> mPathMaps;

	Timer<float> updateTimer;
	int pathUpdateRequests;
	int pathUpdateStaggerCounter;
};