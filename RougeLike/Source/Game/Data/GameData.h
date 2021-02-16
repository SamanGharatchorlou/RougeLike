#pragma once

class GameController;
class Window;
class InputManager;
class RenderManager;
class UIManager;
class Environment;
class ScoreManager;

class NetworkManager;


struct GameData
{
	void setWindow(Window* newWindow);
	void init(GameController* gameController);
	void preLoad();
	void load();
	void setupObservers();
	void free();

	bool endLoading();

	Window* window = nullptr;

	InputManager* inputManager = nullptr;
	RenderManager* renderManager = nullptr;
	UIManager* uiManager = nullptr;
	ScoreManager* scoreManager = nullptr;

	Environment* environment = nullptr;

	NetworkManager* network = nullptr;
};
