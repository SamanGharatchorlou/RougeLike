#pragma once

#include "Collisions/Collider.h"

class Texture;
class PlayerManager;

class Collectable
{
public:
	Collectable() : mTexture(nullptr) { }

	virtual ~Collectable() { };

	void init(std::string value, Texture* texture, RectF rect);
	void render();
	
	virtual void activate(PlayerManager* playerManager) = 0;

	void setPosition(VectorF position) { mRect.SetCenter(position); }

	bool pickedUp() { return mCollider.hasCollided(); }


protected:
	Texture* mTexture;

	Collider mCollider;

	RectF mRect;

	std::string mValue;
};



class WeaponCollectable : public Collectable
{
public:
	WeaponCollectable(std::string value, Texture* texture);

	void activate(PlayerManager* playerManager) override;

};

class HealthCollectable : public Collectable
{
public:

	void activate(PlayerManager* playerManager) override;
};