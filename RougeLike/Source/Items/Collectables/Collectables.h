#pragma once

struct GameData;
class Collectable;

class Collectables
{
public:
	enum ItemType
	{
		MeleeWeapon
	};


public:
	Collectables(GameData* gameData);

	void slowUpdate(float dt);
	void render();

	void spawnRandomItem(ItemType itemType);


private:
	void spawn(Collectable* collectable, VectorF position);
	std::vector<std::string> itemNames(ItemType type);


private:
	GameData* mGameData;

	Timer<float> timer;

	std::vector<Collectable*> mCollectables;
};