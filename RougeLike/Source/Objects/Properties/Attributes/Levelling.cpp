#include "pch.h"
#include "Levelling.h"

#include "Animations/AnimationReader.h"
#include "Game/Camera/Camera.h"


Levelling::Levelling() : mLevel(1), mCurrentExp(0), mRequiredExp(100) { }


void Levelling::init(RectF rect)
{
	XMLParser parser(FileManager::Get()->findFile(FileManager::Config_Player, "LevelUp"));

	AnimationReader reader;
	XMLNode root = parser.rootNode();
	mAnimator = reader.buildAnimator(root.child("Animator"));
	mAnimator.selectAnimation(Action::Active);

	// size
	float targetWidth = rect.Width() * atof(root.child("WidthMultiplier").value().c_str());
	float targetHeight = targetWidth * mAnimator.frameSize().y / mAnimator.frameSize().x;
	VectorF size = VectorF(targetWidth, targetHeight);
	mRect.SetSize(size);

	// render offset
	XMLNode renderOffsetNode = root.child("RenderOffsetRatio");
	StringMap offsetData = renderOffsetNode.attributes();
	mRenderOffset = offsetData.getVector("x", "y") * size;
}


void Levelling::slowUpdate(float dt)
{
	mAnimator.slowUpdate(dt);

	if (mAnimator.loops() == 1)
	{
		mAnimator.reset();
		mAnimator.stop();
	}
}

void Levelling::render(const RectF& playerRect)
{
	if (mAnimator.isRunning())
	{
		mRect.SetBotCenter(playerRect.BotCenter());
		mRect = Camera::Get()->toCameraCoords(mRect.Translate(mRenderOffset));
		mAnimator.render(mRect);
	}
}


void Levelling::gainExp(int exp)
{
	mCurrentExp += exp;

	if (mCurrentExp >= mRequiredExp)
	{
		mCurrentExp -= mRequiredExp;
		mLevelledUp.push(++mLevel);
		mAnimator.start();
	}
}


int Levelling::popLevlledUpEvent()
{
	return mLevelledUp.popFront();
}


bool Levelling::didLevelUp()
{
	return mLevelledUp.size() > 0;
}