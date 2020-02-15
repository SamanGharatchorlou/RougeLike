#pragma once

#include "Health.h"
#include "Damage.h"



// TODO: turn this into a map of <string, property>
template<typename T>
class Property
{
public:
	void set(T property) { mProperty = property; }
	T& get() { return mProperty; }

private:
	T mProperty;
};


class TackleSpeedProperty : public Property<float>
{

};

class TackleDistanceProperty : public Property<float>
{

};

class TackleChangeTimeProperty : public Property<float>
{

};

class HealthProperty : public Property<Health>
{

};

class MovementSpeedProperty : public Property<float>
{

};

class DamageProperty : public Property<Damage>
{

};

class SightRangeProperty : public Property<float>
{

};

class ChaseRangeProperty : public Property<float>
{

};

class HurtTimeProperty : public Property<float>
{

};

class AttentionTimeProperty : public Property<float>
{

};

class ScoreProperty : public Property<int>
{

};