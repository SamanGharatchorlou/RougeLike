#pragma once

#include "CollectableSpawner.h"
#include "CollectableBuilder.h"

struct GameData;
class Collectable;
class CollisionManager;
class Map;
class PlayerManager;

class Collectables
{
public:
	Collectables(GameData* gameData);

	void setCollector(PlayerManager* collector);

	void spawn(const XMLParser& parser, const Map* map);

	void load();

	void slowUpdate(float dt);
	void render();


private:
	//void oscillate(Collectable* collectable, float dt);


private:
	PlayerManager* mCollector;
	CollisionManager* mCollisions;

	//Timer<float> timer;
	CollectableSpawner mSpawner;
	CollectableBuilder mBuilder;

	std::vector<Collectable*> mCollectables;
};