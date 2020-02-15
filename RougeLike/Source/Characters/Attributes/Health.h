#pragma once

class Health
{
public:
	Health() : hp(0) { }
	Health(float health) : hp(health) { }

	void set(float health) { hp = health; }
	float get() { return hp; }

	bool isDead() { return hp <= 0; }
	void takeDamage(float damage) { hp -= damage; }

private:
	float hp;
};