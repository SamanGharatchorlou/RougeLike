#pragma once

class Window;
class TextureManager;
class AudioManager;
class InputManager;
class RenderManager;
class UIManager;
class Environment;
class Player;
class EnemyManager;
class ScoreManager;
class CollisionManager;


struct GameData
{
	void init();
	void setupObservers();
	void free();

	Window* window = nullptr;

	TextureManager* textureManager = nullptr;
	AudioManager* audioManager = nullptr;
	InputManager* inputManager = nullptr;
	RenderManager* renderManager = nullptr;
	UIManager* uiManager = nullptr;
	ScoreManager* scoreManager = nullptr;
	CollisionManager* collisionManager = nullptr;

	Environment* environment = nullptr;

	Player* player = nullptr;
	EnemyManager* enemies = nullptr;
};
