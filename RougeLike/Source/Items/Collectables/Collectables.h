#pragma once

struct GameData;
class Texture;
class Collectable;

class Collectables
{
public:
	Collectables(GameData* gameData);

	void slowUpdate(float dt);
	void render();

	void spawn(Collectable* collectable, int xPosition);


private:
	//std::vector<std::string> itemNames(ItemType type);

	ValueMap getConfigInfo(Collectable* collectable) const;
	Texture* findIcon(Collectable* collectable) const;


private:
	GameData* mGameData;

	Timer<float> timer;

	std::vector<Collectable*> mCollectables;
};