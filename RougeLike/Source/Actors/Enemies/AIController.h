#pragma once

#include "AI/AIPathMap.h"
#include "Events/Dispatcher.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif

class Enemy;
class Map;

class AIController : public Dispatcher
{
public:
	AIController();

	void clear();

	void addMap(const Map* map);
	AIPathMap* popMap();

	void clearCostMaps();

	void updatePaths(std::vector<Enemy*> enemies);
	void updateAIPathCostMap(std::vector<Enemy*> enemies);
	void addPathUpdateRequest() { pathUpdateRequests++; }

	const AIPathMap* pathMap() const { return mPathMaps.at(0); }
	AIPathMap* pathMap(const Map* map);

	int mapCount() const { return mPathMaps.size(); }

#if DRAW_AI_PATH_COSTMAP
	void drawCostMap();
#endif


private:
	void recalculateEnemyPaths(std::vector<Enemy*> enemies);

private:
	//AIPathMap mPathMap;
	std::vector<AIPathMap*> mPathMaps;

	Timer<float> updateTimer;
	int pathUpdateRequests;
	int pathUpdateStaggerCounter;
};