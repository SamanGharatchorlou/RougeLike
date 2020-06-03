#pragma once

#include "Collisions/Collider.h"

class Texture;
class Player;
class Ability;

class Collectable
{
public:
	Collectable();
	Collectable(Texture* icon);
	virtual ~Collectable() { };

	void render(RectF cameraRect) const;
	
	virtual void activate(Player* Player) = 0;

	void setIcon(Texture* icon);

	RectF rect() const { return mRect; }
	void setPosition(VectorF position) { mRect.SetCenter(position); }
	void move(VectorF translation) { mRect = mRect.Translate(translation); }

	Collider* collider() { return &mCollider; }
	bool pickedUp() { return mCollider.gotHit(); }

#if _DEBUG
	RectF colliderRect() const { return mCollider.scaledRect(); }
#endif

protected:
	Texture* mIcon;

	RectF mRect;
	Collider mCollider;
};



class WeaponCollectable : public Collectable
{
public:
	WeaponCollectable(const std::string& value, Texture* texture);

	void activate(Player* Player) override;


private:
	std::string mName;

};


class AbilityCollectable : public Collectable
{
public:
	AbilityCollectable(const std::string& ability, Texture* icon);

	void activate(Player* Player) override;

private:
	std::string mName;
	Ability* mAbility;
};


//class HealthCollectable : public Collectable
//{
//public:
//	void activate(Player* Player) override;
//};