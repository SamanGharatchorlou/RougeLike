#include "pch.h"
#include "Effect.h"

#include "Objects/Actors/Actor.h"
#include "Map/Map.h"

bool canMove(const Actor* actor, VectorF velocity, float dt)
{
	bool validMovement = true;
	RectF rect = actor->scaledRect().Translate(velocity * dt);
	VectorF points[4]{ rect.TopLeft(), rect.TopRight(), rect.BotRight(), rect.BotLeft() };

	for (int i = 0; i < 4; i++)
	{
		if (!isValidFloor(actor->currentMap(), points[i]))
			validMovement = false;
	}

	return validMovement;
}


bool isValidFloor(const Map* map, VectorF point)
{
	return map->isValidPosition(point) && map->tile(point)->is(CollisionTile::Floor);
}