#include "pch.h"
#include "EnemyDead.h"

#include "Objects/Actors/Enemies/Enemy.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"



EnemyDead::EnemyDead(Enemy* enemy) : EnemyState(enemy), mAlpha(alphaMax) { }


void EnemyDead::init()
{
	mEnemy->animator().selectAnimation("Dead");

	// Let everyone know its died
	const float score = mEnemy->getPropertyValue("Score");
	const float exp = mEnemy->getPropertyValue("Experience");

	EnemyDeadEvent* dataPtr = new EnemyDeadEvent(mEnemy, score, exp);
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
	mEnemy->animator().getSpriteTile()->render(rect, mEnemy->physics()->flip(), mAlpha);
#endif

}