#pragma once

#include "Collisions/Collider.h"

class Texture;
class PlayerManager;

class Collectable
{
public:
	Collectable(std::string value) : mValue(value), mTexture(nullptr) { }

	virtual ~Collectable() = 0;

	void init(Texture* texture, RectF rect);
	
	virtual void activate(PlayerManager* playerManager) = 0;

	void render();

	bool pickedUp() { mCollider.hasCollided(); }


public:
	Texture* mTexture;

	Collider mCollider;

	RectF mRect;

	std::string mValue;
};



class WeaponCollectable : public Collectable
{
public:
	void activate(PlayerManager* playerManager) override;
};

class HealthCollectable : public Collectable
{
public:
	void activate(PlayerManager* playerManager) override;
};