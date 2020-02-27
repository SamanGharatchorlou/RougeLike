#include "pch.h"
#include "Collectables.h"



void Collectables::render()
{
	for (unsigned int i = 0; i < mCollectables.size(); i++)
	{
		mCollectables[i].render();
	}
}