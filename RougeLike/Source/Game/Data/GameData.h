#pragma once

class Window;
class AudioManager;
class InputManager;
class RenderManager;
class UIManager;
class Environment;
class ScoreManager;
class CollisionManager;


struct GameData
{
	void setWindow(Window* newWindow);
	void load();
	void setupObservers();
	void free();

	Window* window = nullptr;

	AudioManager* audioManager = nullptr;
	InputManager* inputManager = nullptr;
	RenderManager* renderManager = nullptr;
	UIManager* uiManager = nullptr;
	ScoreManager* scoreManager = nullptr;
	CollisionManager* collisionManager = nullptr;

	Environment* environment = nullptr;
};