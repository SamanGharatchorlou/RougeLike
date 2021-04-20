#pragma once

#include "Animations/Animator.h"

class PlayerManager;
enum class AbilityType;


class Levelling
{
public:
	Levelling();
	~Levelling();

	void init(VectorF size);
	void reset();

	void slowUpdate(float dt);
	void render(const RectF& playerRect);

	bool gainExp(PlayerManager* player, int exp);

	int getCurrentExp() const { return mCurrentExp; }
	int level() const { return mLevel; }

	float expProgress() const { return (float)mCurrentExp / (float)mRequiredExp; }

#if UNLOCK_ALL_ABILITIES
	void unlockAllAbilities(PlayerManager* player);
#endif


private:
	void buildAnimator(const XMLNode& node, VectorF size);
	void levelUp(PlayerManager* player);


private:
	int mLevel;
	int mCurrentExp;
	int mRequiredExp;

	RectF mRect;
	VectorF mRenderOffset;
	Animator mAnimator;

	UniqueQueue<AbilityType> mLockedAbilities;
};