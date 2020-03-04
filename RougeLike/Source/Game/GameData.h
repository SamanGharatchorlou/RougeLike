#pragma once

class Window;
class TextureManager;
class AudioManager;
class InputManager;
class RenderManager;
class UIManager;
class Map;
class Cursor;
class PlayerManager;
class EnemyManager;
class ScoreManager;


struct GameData
{
	void init();
	void free();

	Window* window = nullptr;

	TextureManager* textureManager = nullptr;
	AudioManager* audioManager = nullptr;
	InputManager* inputManager = nullptr;
	RenderManager* renderManager = nullptr;
	UIManager* uiManager = nullptr;
	ScoreManager* scoreManager = nullptr;

	Map* map = nullptr;
	Cursor* cursor = nullptr;

	PlayerManager* playerManager = nullptr;
	EnemyManager* enemies = nullptr;
};

