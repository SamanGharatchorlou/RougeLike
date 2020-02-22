#include "pch.h"
#include "PlayerPropertyBag.h"
#include "System/Files/StatReader.h"


void PlayerPropertyBag::readAttributes(std::string name)
{
	StatReader statReader;
	XMLValueMap map = statReader.getStats(FileManager::Config_Stats_Player, name);

	// Health
	Health health(map.getFloat("Health"));
	pHealth.set(health);

	// Force
	float force = map.getFloat("Force");
	pForce.set(force);

	// Max velocity
	float maxVelocity = map.getFloat("MaxVelocity");
	pMaxVelocity.set(maxVelocity);

	// Drag force
	float dragFactor = map.getFloat("DragFactor");
	pDragFactor.set(dragFactor);
}
