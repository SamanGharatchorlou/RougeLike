#pragma once

#include "Health.h"
#include "Damage.h"



// TODO: turn this into a map of <string, property>
template<typename T>
class Property
{
public:
	void set(T property) { mProperty = property; }
	const T& get() const { return mProperty; }

private:
	T mProperty;
};

// Shared
class HealthProperty : public Property<Health> { };

// Player
class ForceProperty : public Property<float> { };

class MaxVelocityProperty : public Property<float> { };

class DragFactorProperty : public Property<float> { };




// Enemy
class MovementSpeedProperty : public Property<float> { };

class TackleSpeedProperty : public Property<float> { };

class TackleDistanceProperty : public Property<float> { };

class TackleChangeTimeProperty : public Property<float> { };


class DamageProperty : public Property<Damage> { };

class SightRangeProperty : public Property<float> { };

class ChaseRangeProperty : public Property<float> { };

class HurtTimeProperty : public Property<float> { };

class AttentionTimeProperty : public Property<float> { };

class ScoreProperty : public Property<int> { };