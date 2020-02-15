#pragma once

class Collider;

class ColliderManager
{
public:
	void addCollider(Collider* collider);
	void subscribe(Collider* collider);

	void update();

private:
	std::vector<Collider*> mBaseColliders;
	std::vector<Collider*> mSubbedColliders;
};
