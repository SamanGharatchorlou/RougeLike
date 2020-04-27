#include "pch.h"
#include "Collectables.h"

#include "Game/Camera.h"
#include "Game/GameData.h"


Collectables::Collectables(GameData* gameData) : mGameData(gameData) 
{ 
	timer.start();
}


// Collectables now owns this object, must be destroyed.
void Collectables::spawn(Collectable* collectable, VectorF position)
{
	collectable->setPosition(position);
	mCollectables.push_back(collectable);
	mCollisionTracker.addDefender(collectable->getCollider());
}


void Collectables::slowUpdate(float dt)
{
	mCollisionTracker.checkDefenderCollisions();

	float oscillation = std::sin(timer.getSeconds() * dt * 500);
	VectorF oscillationVector = VectorF(oscillation, oscillation);

	std::vector<Collectable*>::iterator iter;
	for (iter = mCollectables.begin(); iter != mCollectables.end(); iter++)
	{
		(*iter)->move(oscillationVector);

		if ((*iter)->pickedUp())
		{
			(*iter)->activate(mGameData->playerManager);
			
			// Destroy the collectable
			mCollectables.erase(iter);

			// TODO: how to delete this???
			//delete *iter;
			break;
		}
	}
}


void Collectables::render()
{
	for (unsigned int i = 0; i < mCollectables.size(); i++)
	{
		RectF worldRect = mCollectables[i]->rect();

		if (Camera::Get()->inView(worldRect))
		{
			RectF cameraRect = Camera::Get()->toCameraCoords(worldRect);
#if DRAW_COLLECTABLE_RECT
			debugDrawRect(mCollectables[i]->colliderRect(), RenderColour::Blue);
			debugDrawRect(worldRect, RenderColour::Green);
#else
			mCollectables[i]->render(cameraRect);
#endif
		}
	}
}


void Collectables::subscribeCollider(Collider* collider)
{
	mCollisionTracker.addAttacker(collider);
}