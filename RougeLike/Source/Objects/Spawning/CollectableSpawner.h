#pragma once


#include "Items/Spawner.h"
#include "Items/Collectables/Collectable.h"

using CollectablesMap = std::map<CollectableType, Attributes>;

class CollectableSpawner : public Spawner
{

public:
	CollectablesMap readLevelData(const XMLParser& parser);

private:

};