#pragma once


class Damage
{
public:
	Damage() : attackDamage(0) { }
	Damage(float dmg) : attackDamage(dmg) { }

	void set(float damage) { attackDamage = damage; }
	const float get() const { return attackDamage; }

	inline Damage operator + (Damage damage) const
	{
		return Damage(attackDamage + damage.get());
	}

	inline Damage& operator += (Damage damage)
	{
		attackDamage += damage.get();
		return *this;
	}

	// TODO: add effect damage e.g. fire, ice etc
private:
	float attackDamage;
};