#pragma once


class PropertyType
{
public:
	enum Type
	{
		None,

		// StringMap
		Damage,
		Health,
		Armor,
		Level,

		// Physics
		Force,
		Distance,
		MaxVelocity,
		Velocity,
		Collider,

		Time,

		xPosition,
		yPosition,

		// Effect values
		DamageReact,
		KnockbackForce,
		KnockbackDistance,

		FallSpeed,
		StunTime,

		// Enemy
		Score,
		Experience,
		SightRange,
		TackleDistance,
		ChaseRange,
		AttackSpeed,
		TackleMovementSpeed,

		// Ability
		MaxSize,
		Cooldown,
		Range,
	};

public:
	PropertyType(PropertyType::Type propertyType) : type(propertyType) { }
	PropertyType(const BasicString& name);

	BasicString string() const;

	const Type type;


protected:
	Type getType(const BasicString& name) const;
};




class AttributeType : public PropertyType
{
public:
	AttributeType(PropertyType::Type propertyType);
	AttributeType(const BasicString& name);
};



PropertyType::Type findPropertyType(const BasicString& string);
BasicString findPropertyString(PropertyType::Type type);
static const std::unordered_map<BasicString, PropertyType::Type> PropertyLookUpTable
{
	{ "None", PropertyType::None },

	// StringMap
	{ "Damage", PropertyType::Damage },
	{ "Health", PropertyType::Health },
	{ "Armor", PropertyType::Armor },
	{ "Level", PropertyType::Level },

	// Physics
	{ "Force", PropertyType::Force },
	{ "Distance", PropertyType::Distance },
	{ "MaxVelocity", PropertyType::MaxVelocity },
	{ "Velocity", PropertyType::Velocity },
	{ "Collider", PropertyType::Collider },

	{ "yPosition", PropertyType::xPosition },
	{ "xPosition", PropertyType::yPosition },

	{ "Time", PropertyType::Time},

	// Effect values
	{ "KnockbackForce", PropertyType::KnockbackForce },
	{ "KnockbackDistance", PropertyType::KnockbackDistance },
	{ "StunTime", PropertyType::StunTime},

	// Enemy
	{ "Score", PropertyType::Score },
	{ "Experience", PropertyType::Experience },
	{ "SightRange", PropertyType::SightRange },
	{ "TackleDistance", PropertyType::TackleDistance },
	{ "ChaseRange", PropertyType::ChaseRange },
	{ "AttackSpeed", PropertyType::AttackSpeed },
	{ "TackleMovementSpeed", PropertyType::TackleMovementSpeed },

	{ "MaxSize", PropertyType::MaxSize },
	{ "Cooldown", PropertyType::Cooldown },
	{ "Range", PropertyType::Range },
};


bool operator == (PropertyType propertyA, PropertyType propertyB);
bool operator == (AttributeType attributeA, AttributeType attributeB);


namespace std
{
	template <>
	struct hash<PropertyType>
	{
		std::size_t operator()(PropertyType property) const
		{
			unsigned long propertyType = (unsigned long)property.type;
			return std::hash<unsigned long>{ }(propertyType);
		}
	};

	template <>
	struct hash<AttributeType>
	{
		std::size_t operator()(AttributeType attribute) const
		{
			unsigned long attributeType = (unsigned long)attribute.type;
			return std::hash<unsigned long>{ }(attributeType);
		}
	};
}