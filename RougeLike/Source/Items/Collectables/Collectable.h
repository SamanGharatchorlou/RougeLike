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
	void render(RectF rect) const;
	
	virtual void activate(PlayerManager* playerManager) = 0;

	RectF rect() const { return mRect; }

	void setPosition(VectorF position) { mRect.SetCenter(position); }
	void move(VectorF translation) { mRect.Translate(translation); }

	Collider* getCollider() { return &mCollider; }
	//bool pickedUp() { return mCollider.hasCollided(); }


protected:
	Texture* mTexture;

	Collider mCollider;

	RectF mRect;

	std::string mValue;
};



class WeaponCollectable : public Collectable
{
public:
	WeaponCollectable(const std::string& value, Texture* texture);

	void activate(PlayerManager* playerManager) override;

};

class HealthCollectable : public Collectable
{
public:

	void activate(PlayerManager* playerManager) override;
};