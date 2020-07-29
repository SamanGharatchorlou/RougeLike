#pragma once


#include "AI/AIPathMap.h"

#if _DEBUG
#include "Debug/DebugDraw.h"
#endif

class Enemy;
class Map;

class AIController
{
public:
	AIController();

	void clearMap() { mPathMap.clear(); }

	void loadAIPathMap(const Map* map);

	void updatePaths(std::vector<Enemy*>& enemies);

	// AI pathing
	void updateAIPathCostMap(std::vector<Enemy*>& enemies);
	void addPathUpdateRequest() { pathUpdateRequests++; }

	const AIPathMap* pathMap() const { return &mPathMap; }

#if DRAW_AI_PATH_COSTMAP
	void drawCostMap();
#endif


private:
	void recalculateEnemyPaths(std::vector<Enemy*>& enemies);

private:
	AIPathMap mPathMap;

	Timer<float> updateTimer;
	int pathUpdateRequests;
	int pathUpdateStaggerCounter;
};