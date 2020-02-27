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

	Property& operator += (Property propertry)
	{
		T = T + propertry.get();
		return *this;
	}

private:
	T mProperty;
};



// Shared
class HealthProperty : public Property<Health> { };
class MovementSpeedProperty : public Property<float> { };
class AttackDamageProperty : public Property<Damage> { };

// Player - Attributes
class LevelProperty : public Property<Level> { };
class DefenceProperty : public Property<float> { };
class AttackSpeedProperty : public Property<double> { };

// Player - Level up rate
class HealthUpRate : public Property<Health> { };
class DefenceUpRate : public Property<float> { };
class AttackDamageUpRate : public Property<Damage> { };
class AttackSpeedUpRate : public Property<double> { };
class MovementSpeedUpRate : public Property<float> { };

// Player - Movement
class ForceProperty : public Property<float> { };
class MaxVelocityProperty : public Property<float> { };
class DragFactorProperty : public Property<float> { };

// Player - Other
class WeaponProperty : public Property<std::string> { };


// Enemy
class TackleSpeedProperty : public Property<float> { };
class TackleDistanceProperty : public Property<float> { };
class TackleChangeTimeProperty : public Property<float> { };
class SightRangeProperty : public Property<float> { };
class ChaseRangeProperty : public Property<float> { };
class HurtTimeProperty : public Property<float> { };
class AttentionTimeProperty : public Property<float> { };
class ScoreProperty : public Property<int> { };
class ExperienceProperty : public Property<int> { };