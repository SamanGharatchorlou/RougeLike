#pragma once

class GameController;
class Window;
class InputManager;
class RenderManager;
class UIManager;
class Environment;
class ScoreManager;
class CollisionManager;


struct GameData
{
	// TODO: move this logic somewhere else?
	void setWindow(Window* newWindow);
	void init(GameController* gameController);
	void preLoad();
	void load();
	void setupObservers();
	void free();

	Window* window = nullptr;

	InputManager* inputManager = nullptr;
	RenderManager* renderManager = nullptr;
	UIManager* uiManager = nullptr;
	ScoreManager* scoreManager = nullptr;
	CollisionManager* collisionManager = nullptr;

	Environment* environment = nullptr;
};
