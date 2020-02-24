#pragma once

#include "Health.h"
#include "Damage.h"
#include "Level.h"


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



// Shared
class HealthProperty : public Property<Health> { };

// Player - Attributes
class LevelProperty : public Property<Level> { };
class DefenceProperty : public Property<float> { };
class AttackDamageProperty : public Property<float> { };
class AttackSpeedProperty : public Property<float> { };
class MovementSpeedPropert : public Property<float> { };

// Player - Movement
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
class ExperienceProperty : public Property<int> { };