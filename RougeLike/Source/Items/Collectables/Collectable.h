#pragma once

#include "Collisions/Collider.h"

class Texture;
class PlayerManager;

class Collectable
{
public:
	Collectable() : mTexture(nullptr) { }

	void init(Texture* texture, RectF rect, std::string id);
	
	virtual void activate(PlayerManager* playerManager) = 0;

	void render();

	bool pickedUp() { mCollider.hasCollided(); }


public:
	Texture* mTexture;

	Collider mCollider;

	RectF mRect;

	std::string mId;
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