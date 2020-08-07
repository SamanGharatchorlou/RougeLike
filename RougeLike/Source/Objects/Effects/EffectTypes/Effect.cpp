#include "pch.h"
#include "Effect.h"

#include "Actors/Actor.h"
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


void setValue(const BasicString& label, float& value, const ValueBag& valueBag)
{
	if (valueBag.contains(label))
		value = valueBag.get(label);
	else
		DebugPrint(Log, "No value labeled '%s' was found in the valueBag, cannot set value. Check the XML config\n", label.c_str());
}

void setValue(const BasicString& label, Property& value, const ValueBag& valueBag)
{
	if (valueBag.contains(label))
		value.setValue(valueBag.get(label));
	else
		DebugPrint(Log, "No value labeled '%s' was found in the valueBag, cannot set value. Check the XML config\n", label.c_str());
}