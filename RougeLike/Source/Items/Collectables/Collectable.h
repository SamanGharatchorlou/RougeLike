#pragma once

#include "Collisions/Collider.h"


class Texture;
class Player;
class Ability;


class Collectable
{
public:
	enum class Type
	{
		MeleeWeapon,
		Ability
	};

public:
	Collectable();
	virtual ~Collectable() { };

	void render(RectF cameraRect) const;
	
	virtual void activate(Player* Player) = 0;

	void setIcon(Texture* icon);

	const std::string& name() const { return mName; }

	RectF rect() const { return mRect; }
	void setPosition(VectorF position) { mRect.SetCenter(position); }
	void move(VectorF translation) { mRect = mRect.Translate(translation); }

	Collider* collider() { return &mCollider; }
	bool pickedUp() { return mCollider.gotHit(); }

	virtual Type type() const = 0;

#if _DEBUG
	RectF colliderRect() const { return mCollider.scaledRect(); }
#endif

protected:
	std::string mName;

	Texture* mIcon;

	RectF mRect;
	Collider mCollider;
};



class WeaponCollectable : public Collectable
{
public:
	WeaponCollectable(const std::string& value);

	void activate(Player* Player) override;

	Type type() const override { return Type::MeleeWeapon; }
};


class AbilityCollectable : public Collectable
{
public:
	AbilityCollectable(const std::string& ability);

	void activate(Player* Player) override;

	Type type() const override { return Type::Ability; }
};

