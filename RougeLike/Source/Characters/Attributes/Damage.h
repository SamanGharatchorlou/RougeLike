#pragma once

class Damage
{
public:
	Damage() : attackDamage(0) { }
	Damage(float dmg) : attackDamage(dmg) { }

	void set(float damage) { attackDamage = damage; }
	const float get() const { return attackDamage; }

	// TODO: add effect damage e.g. fire, ice etc
private:
	float attackDamage;
};