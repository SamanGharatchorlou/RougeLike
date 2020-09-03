#include "pch.h"
#include "CollectableBuilder.h"

#include "Graphics/TextureManager.h"


void CollectableBuilder::loadPool()
{
	std::vector<CollectableType> types;
	for (CollectableType type = CollectableType::None + 1; type < CollectableType::Count; type = type + 1)
	{
		types.push_back(type);
	}

	mPool.load(types, 5);
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


	const TextureManager* textures = TextureManager::Get();
	Texture* icon = textures->getTexture(iconName, FileManager::Image_UI);

	if(!icon)
		DebugPrint(Warning, "Collectable '%s' info has no valid icon name\n", data.id.c_str());

	collectable->setIcon(icon);
}


void CollectableBuilder::returnCollectable(Collectable* collectable)
{
	mPool.returnObject(collectable, collectable->type());
}