#include "pch.h"
#include "CollectableBuilder.h"

#include "Graphics/TextureManager.h"


void CollectableBuilder::loadPool()
{
	mPool.load();
}


Collectable* CollectableBuilder::build(CollectableSpawner::SpawnData data)
{
	Collectable* collectable = mPool.getObject(data.type);

	collectable->set(data.id);
	collectable->setPosition(data.position);
	setIcon(collectable, data);

	return collectable;
}


void CollectableBuilder::setIcon(Collectable* collectable, CollectableSpawner::SpawnData data)
{
	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs_Objects, data.id));
	BasicString iconName = parser.rootChild("Icon").value();

	Texture* icon = mTextures->getTexture(iconName, FileManager::Image_UI);

	if(!icon)
		DebugPrint(Log, "Collectable '%s' info has no valid icon name\n", data.id.c_str());

	collectable->setIcon(icon);
}


void CollectableBuilder::returnCollectable(Collectable* collectable)
{
	mPool.returnObject(collectable);
}