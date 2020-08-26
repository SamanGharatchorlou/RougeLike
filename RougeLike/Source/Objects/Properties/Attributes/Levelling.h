#pragma once

#include "Animations/Animator.h"

class PlayerManager;
enum class AbilityType;


class Levelling
{
public:
	Levelling();
	~Levelling();

	void init(const XMLNode& levelNode, RectF rect);
	void reset();

	void slowUpdate(float dt);
	void render(const RectF& playerRect);

	bool gainExp(PlayerManager* player, int exp);

	int getCurrentExp() const { return mCurrentExp; }
	int level() const { return mLevel; }


#if UNLOCK_ALL_ABILITIES
	void unlockAllAbilities(PlayerManager* player);
#endif


private:
	void buildAnimator(const BasicString& infoFile, RectF rect);
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