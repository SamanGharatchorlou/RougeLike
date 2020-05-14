#pragma once

struct GameData;
class Environment;
class ActorManager;
class UIManager;
class Collectables;

// TODO: implement
//#include "Renderable.h"

class RenderManager
{
public:
	RenderManager(GameData* gameData);

	void Set(Environment* environment) { mEnvironment = environment; }
	void Set(ActorManager* actors) { mActors = actors; }
	void Set(UIManager* UI) { mUIManager = UI; }
	void Set(Collectables* collectables) { mCollectables = collectables; }

	//void add(Renderable* renderable);

	void render();

private:
	GameData* mGameData;

	Environment* mEnvironment;
	ActorManager* mActors;
	UIManager* mUIManager;
	Collectables* mCollectables;

	//std::vector<Renderable*> renderables;
};
