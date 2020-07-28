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

//EffectType stringToEffectType(const std::string effect)
//{
//	if (effect == "Damage")
//		return EffectType::Damage;
//	else
//		return EffectType::None;
//}

inline void operator >>(EffectType a, std::string& str)
{
	if (a == EffectType::Damage)
		str = "Damage";
}


inline void operator <<(EffectType& a, const std::string& str)
{
	if (str == "Damage")
		a = EffectType::Damage;
}