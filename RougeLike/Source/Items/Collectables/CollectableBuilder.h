#pragma once

#include "CollectableSpawner.h"
#include "CollectablesPool.h"

class TextureManager;

class CollectableBuilder
{
public:
	CollectableBuilder(TextureManager* textures) : mTextures(textures) { }

	void loadPool();

	Collectable* build(CollectableSpawner::SpawnData data);
	void returnCollectable(Collectable* collectable);


private:
	void setIcon(Collectable* collectable, CollectableSpawner::SpawnData data);


protected:
	TextureManager* mTextures;

	CollectablesPool mPool;
};