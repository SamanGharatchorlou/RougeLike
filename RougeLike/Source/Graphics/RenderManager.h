#pragma once

class Environment;
class ActorManager;
class UIManager;
class Collectables;

class RenderManager
{
public:
	RenderManager();

	void Set(Environment* environment) { mEnvironment = environment; }
	void Set(ActorManager* actors) { mActors = actors; }
	void Set(UIManager* UI) { mUIManager = UI; }
	void Set(Collectables* collectables) { mCollectables = collectables; }

	void render();

private:
	Environment* mEnvironment;
	ActorManager* mActors;
	UIManager* mUIManager;
	Collectables* mCollectables;
};
