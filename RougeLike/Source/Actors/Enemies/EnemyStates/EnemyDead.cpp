#include "pch.h"
#include "EnemyDead.h"

#include "Actors/Enemies/Enemy.h"

#include "Game/Camera.h"
#include "Graphics/Texture.h"



EnemyDead::EnemyDead(Enemy* enemy) : EnemyState(enemy), mAlpha(alphaMax) { }


void EnemyDead::init()
{
	mEnemy->animator().selectAnimation(Action::Dead);

	// Let everyone know its died
	const float score = mEnemy->getPropertyValue("Score");
	const float exp = mEnemy->getPropertyValue("Experience");

	EnemyDeadEvent* dataPtr = new EnemyDeadEvent(mEnemy, score, exp);
	mEnemy->events().push(EventPacket(dataPtr));
}


void EnemyDead::slowUpdate(float dt)
{
	const Animator::Animation& anim = mEnemy->animator().animation();

	if (anim.currentFrame() == anim.frameCount())
	{
		mEnemy->animator().stop();
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