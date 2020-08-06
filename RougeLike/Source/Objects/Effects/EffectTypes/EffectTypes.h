#pragma once

enum class EffectType
{
	None,

	Armor,
	Heal,

	Blink,

	Damage,
	Displacement,
	Stun,
	KnockbackStun,

	Count
};

template<class T>
inline EffectType operator +(EffectType a, T b)
{
	int sum = static_cast<int>(a) + static_cast<int>(b);
	return static_cast<EffectType>(sum);
}


// Type to string
inline void operator >>(EffectType a, BasicString& str)
{
	if (a == EffectType::Damage)
		str = "Damage";
}

// String to type
inline void operator <<(EffectType& a, const BasicString& str)
{
	if (str == "Damage")
		a = EffectType::Damage;
	else if (str == "Displacement")
		a = EffectType::Displacement;
}