#pragma once

class Actor;
class Map;
class PropertyMap;
class Attribute;

#include "EffectTypes.h"
#include "Objects/Properties/Property.h"

class Effect
{

public:
	Effect() : mReceiver(nullptr), mShouldExit(false) { }
	virtual ~Effect() { }

	void setReceiver(Actor* receiver) { mReceiver = receiver; }
	
	virtual void clearData() { };
	void clearBaseData() { mReceiver = nullptr; mShouldExit = false; }

	virtual void fill(const PropertyMap& properties) { } // = 0;

	virtual void init() = 0;
	virtual void fastUpdate(float dt) = 0;
	virtual void slowUpdate(float dt) = 0;
	virtual void render() = 0;
	virtual void exit() = 0;

	void endEffect() { mShouldExit = true; }
	bool shouldExit() const { return mShouldExit; }

	virtual EffectType type() const = 0;


protected:
	Actor* mReceiver;
	bool mShouldExit;
};


bool canMove(const Actor* actor, VectorF velocity, float dt);
bool isValidFloor(const Map* map, VectorF point);

void setProperty(PropertyType type, float& value, const PropertyMap& PropertyMap);
void setProperty(PropertyType type, Attribute& value, const PropertyMap& PropertyMap);
