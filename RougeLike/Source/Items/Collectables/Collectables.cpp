#include "pch.h"
#include "Collectables.h"


// Collectables now owns this object, must be destroyed.
void Collectables::spawn(Collectable* collectable, VectorF position)
{
	collectable->setPosition(position);
	mCollectables.push_back(collectable);
}


void Collectables::slowUpdate()
{
	for (std::vector<Collectable*>::iterator iter; iter != mCollectables.end(); iter++)
	{
		if ((*iter)->pickedUp())
		{
			(*iter)->activate(mGameData->playerManager);
			
			// Destroy the collectable
			mCollectables.erase(iter);
			delete *iter;
			break;
		}
	}
}



void Collectables::render()
{
	for (unsigned int i = 0; i < mCollectables.size(); i++)
	{
		mCollectables[i]->render();
	}
}