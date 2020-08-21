#pragma once

#include "Animations/Animator.h"


class Levelling
{
public:
	Levelling();

	void init(RectF rect);

	void slowUpdate(float dt);
	void render(const RectF& playerRect);

	void gainExp(int exp);

	int getCurrentExp() const { return mCurrentExp; }

	bool didLevelUp();
	int popLevlledUpEvent();


private:
	int mLevel;
	int mCurrentExp;
	int mRequiredExp;

	UniqueQueue<int> mLevelledUp;

	RectF mRect;
	VectorF mRenderOffset;

	Animator mAnimator;
};