#pragma once

struct GameData;
class Collectable;

class Collectables
{
public:
	enum ItemType
	{
		MeleeWeapon,
		Ability
	};


public:
	Collectables(GameData* gameData);

	void slowUpdate(float dt);
	void render();

	void spawnRandomItem(ItemType itemType);


	void spawn(Collectable* collectable, float xPosition);

private:
	std::vector<std::string> itemNames(ItemType type);

	void spawn(Collectable* collectable, VectorF position);

private:
	GameData* mGameData;

	Timer<float> timer;

	std::vector<Collectable*> mCollectables;
};