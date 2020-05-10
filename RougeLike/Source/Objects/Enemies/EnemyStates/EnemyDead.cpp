#include "pch.h"
#include "EnemyDead.h"

#include "Objects/Enemies/Enemy.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"



EnemyDead::EnemyDead(Enemy* enemy) : EnemyState(enemy), mAlpha(alphaMax) { }


void EnemyDead::init()
{
	mEnemy->getAnimator()->selectAnimation("Dead");

	// Let everyone know he has died
	int score = mEnemy->propertyBag().value("Score");
	int exp = mEnemy->propertyBag().value("Exp");
	EnemyDeadEvent* dataPtr = new EnemyDeadEvent(score, exp);

	mEnemy->pushEvent(EventPacket(Event::EnemyDead, dataPtr));
}


void EnemyDead::slowUpdate(float dt)
{
	mAlpha -= (250 * dt);

	// Remove enemy from play
	if (mAlpha <= 10)
		mEnemy->addState(EnemyState::None);
}


// Not using Enemy->renderCharacter() so we can adjust the alpha
void EnemyDead::render()
{
	RectF rect = mEnemy->renderRect();
	rect = Camera::Get()->toCameraCoords(rect);

#if DRAW_ENEMY_RECT
	debugDrawRect(mEnemy->rect(), RenderColour(RenderColour::Red));
#else
	mEnemy->getAnimator()->getSpriteTile()->render(rect, mEnemy->flip(), mAlpha);
#endif

}