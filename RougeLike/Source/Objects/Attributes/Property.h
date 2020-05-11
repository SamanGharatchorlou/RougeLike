#pragma once

class Property
{
public:
	Property() { }
	virtual ~Property() { };
	
	virtual const float value() const = 0;
};



class PropertyValue : public Property
{
public:
	PropertyValue(float value) : mValue(value) { }

	void setValue(float value) { mValue = value; }
	const float value() const { return mValue; }

private:
	float mValue;
};



/*

// TODO: turn this into a map of <string, property>
template<typename T>
class Property
{
public:
	void set(T property) { mProperty = property; }

	// TODO: how do I know which get it will use?
	const T get() const { return mProperty; }
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
class MovementSpeedProperty : public Property<int> { };
class AttackDamageProperty : public Property<Damage> { };

// Physics
class ForceProperty : public Property<float> { };
class MaxVelocityProperty : public Property<float> { };
class DragFactorProperty : public Property<float> { };
class MassProperty : public Property<float> { };


// Player - Attributes
class LevelProperty : public Property<Level> { };
class DefenceProperty : public Property<int> { };
class AttackSpeedProperty : public Property<float> { };

// Player - Level up rate
class HealthUpRate : public Property<Health> { };
class DefenceUpRate : public Property<int> { };
class AttackDamageUpRate : public Property<Damage> { };
class AttackSpeedUpRate : public Property<float> { };
class MovementSpeedUpRate : public Property<int> { };

// Player - Other
class WeaponProperty : public Property<std::string> { };


// Enemy

// Enemy - Attack
class TackleSpeedProperty : public Property<float> { };
class TackleDistanceProperty : public Property<float> { };
class TackleChangeTimeProperty : public Property<float> { };
class KnockbackDistanceProperty : public Property<float> { };

// Enemy Patrol
class SightRangeProperty : public Property<float> { };
class ChaseRangeProperty : public Property<float> { };
class IdleTimeProperty : public Property<float> { };

// Enemy - Hurt
class HurtTimeProperty : public Property<float> { };

// Enemy - Alert
class AttentionTimeProperty : public Property<float> { };

// Enemy - Data values
class ScoreProperty : public Property<int> { };
class ExperienceProperty : public Property<int> { };

*/