#include "pch.h"
#include "EnemyDead.h"

#include "Actors/Enemies/Enemy.h"
#include "Game/Camera/Camera.h"


void EnemyDead::init()
{
	mAlpha = alphaMax;

	mEnemy->animator().selectAnimation(Animation::Dead);

	int score = (int)mEnemy->getAttributeValue(AttributeType::Score);
	int exp = (int)mEnemy->getAttributeValue(AttributeType::Experience);

	EnemyDeadEvent* dataPtr = new EnemyDeadEvent(mEnemy, score, exp);
	mEnemy->events().push(EventPacket(dataPtr));
}


void EnemyDead::slowUpdate(float dt)
{
	if (mEnemy->animator().currentFrame() + 1 == mEnemy->animator().frameCount())
	{
		mEnemy->animator().pause();
		mAlpha -= (250 * dt);
	}

	// Remove enemy from play
	if (mAlpha <= 10)
		mEnemy->replaceState(EnemyState::Exit);
}


// Not using Enemy->renderCharacter() so we can adjust the alpha
void EnemyDead::render()
{
	RectF rect = Camera::Get()->toCameraCoords(mEnemy->rect());

#if DRAW_ENEMY_RECT
	debugDrawRect(mEnemy->rect(), RenderColour(RenderColour::Red));
#else
	mEnemy->animator().render(rect, mEnemy->physics()->flip(), (Uint8)mAlpha);
#endif

}