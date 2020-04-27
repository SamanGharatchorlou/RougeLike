#pragma once

// TODO:
// make this inherit from an effect class with some active(dt) method where it does its thing
// physics bodies apply all effects in a list like manner
// can add, speed up, slow down, knockback etc.


class Knockback
{
public:
	Knockback(VectorF source, float force);

	void distance(float dt);

	void end();

private:
	Timer<float> mTimer;

	VectorF source;
	float mForce;
};