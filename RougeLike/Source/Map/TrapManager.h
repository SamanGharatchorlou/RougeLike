#pragma once

class Map;
class Trap;
class DecorMap;
class CollisionManager;
class EffectPool;

class TrapManager
{
public:
	TrapManager(CollisionManager* collisions, EffectPool* effects) : mCollisions(collisions), mEffects(effects) { };

	void addTraps(Map* map, const DecorMap& info);

	void slowUpdate();

	
private:
	void setTraps();
	void triggerTraps();
	void resetTraps();


private:
	CollisionManager* mCollisions;
	EffectPool* mEffects;

	std::vector<Trap*> mTraps;
};