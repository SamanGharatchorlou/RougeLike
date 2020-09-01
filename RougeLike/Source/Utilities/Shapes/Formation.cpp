#include "pch.h"
#include "Formation.h"

#include "AI/Pathing/AIPathMap.h"


bool Formation::isValid(const AIPathMap* map) const
{
	for (const VectorF& point : mPoints)
	{
		Index index = map->index(point);
		if (!map->isValidIndex(index) || !map->floorCollisionTile(index))
			return false;
	}

	return true;
}