#pragma once


enum class EffectType;


// IMPROVEMENT
// Replace this with an ability?
struct AttackingEffects
{
	void fill(const XMLNode& node);
	void clear();

	std::vector<EffectType> mTypes;
	PropertyMap mData;
};