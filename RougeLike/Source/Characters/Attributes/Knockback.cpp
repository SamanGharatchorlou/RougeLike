#include "pch.h"
#include "Knockback.h"

Knockback::Knockback(VectorF source, float force) : mForce(force) { }

void Knockback::distance(float dt)
{

}

void Knockback::end()
{
	// set some bool here so it can be cleaned up by the physics body class;
}