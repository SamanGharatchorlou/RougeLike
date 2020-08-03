#pragma once

#include "Collisions/Collider.h"


class Texture;
class PlayerManager;
class Ability;

enum class CollectableType
{
	None,
	MeleeWeapon,
	Ability,
	Count
};

template<class T>
inline CollectableType operator +(CollectableType a, T b)
{
	int sum = static_cast<int>(a) + static_cast<int>(b);
	return static_cast<CollectableType>(sum);
}

inline void operator <<(CollectableType& a, const BasicString& str)
{
	if (str == "Ability")
		a = CollectableType::Ability;
}


class Collectable
{
public:
	Collectable();
	virtual ~Collectable() { };

	virtual void activate(PlayerManager* Player) = 0;

	void render(RectF cameraRect) const;

	void set(const BasicString& id);
	void setIcon(Texture* icon);

	const BasicString& name() const { return mID; }

	RectF rect() const { return mRect; }
	void setPosition(VectorF position) { mRect.SetCenter(position); }
	void move(VectorF translation) { mRect = mRect.Translate(translation); }

	Collider* collider() { return &mCollider; }
	bool pickedUp() { return mCollider.gotHit(); }

	virtual CollectableType type() const = 0;

#if _DEBUG
	RectF colliderRect() const { return mCollider.scaledRect(); }
#endif

protected:
	BasicString mID;

	Texture* mIcon;

	RectF mRect;
	Collider mCollider;
};



class WeaponCollectable : public Collectable
{
public:
	void activate(PlayerManager* Player) override;

	CollectableType type() const override { return CollectableType::MeleeWeapon; }
};


class AbilityCollectable : public Collectable
{
public:
	void activate(PlayerManager* Player) override;

	CollectableType type() const override { return CollectableType::Ability; }
};

