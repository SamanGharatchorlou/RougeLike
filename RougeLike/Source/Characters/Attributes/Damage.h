#pragma once


class Damage
{
public:
	Damage() : attackDamage(0) { }
	Damage(int dmg) : attackDamage(dmg) { }

	void set(int damage) { attackDamage = damage; }
	const int value() const { return attackDamage; }

	inline Damage operator + (Damage damage) const
	{
		return Damage(attackDamage + damage.value());
	}

	inline Damage& operator += (Damage damage)
	{
		attackDamage += damage.value();
		return *this;
	}

	// TODO: add effect damage e.g. fire, ice etc
private:
	int attackDamage;
};