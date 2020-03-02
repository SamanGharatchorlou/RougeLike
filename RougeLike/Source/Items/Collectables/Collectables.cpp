#include "pch.h"
#include "Collectables.h"

#include "Game/GameData.h"


Collectables::Collectables(GameData* gameData) : mGameData(gameData) { }


// Collectables now owns this object, must be destroyed.
void Collectables::spawn(Collectable* collectable, VectorF position)
{
	collectable->setPosition(position);
	mCollectables.push_back(collectable);
	mCollisionTracker.addCollider(collectable->getCollider());
}


void Collectables::slowUpdate()
{
	mCollisionTracker.checkForCollisions();

	std::vector<Collectable*>::iterator iter;
	for (iter = mCollectables.begin(); iter != mCollectables.end(); iter++)
	{
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
		mCollectables[i]->render(mGameData->camera);
	}
}


void Collectables::subscrbeCollider(Collider* collider)
{
	mCollisionTracker.subscribe(collider);
}