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
	~Collectables();

	void setCollector(PlayerManager* collector);

	void spawn(const XMLNode collectablesNode, const Map* map);

	void load();

	void slowUpdate(float dt);
	void render();


private:
	PlayerManager* mCollector;
	CollisionManager* mCollisions;

	CollectableSpawner mSpawner;
	CollectableBuilder mBuilder;

	std::vector<Collectable*> mCollectables;
};