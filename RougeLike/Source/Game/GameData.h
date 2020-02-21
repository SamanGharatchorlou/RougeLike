#pragma once

class Window;
struct SDL_Renderer;
class TextureManager;
class AudioManager;
class InputManager;
class RenderManager;
class UIManager;
class Map;
class Camera;
class Cursor;
class Player;

class ScoreManager;


struct GameData
{
	Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	TextureManager* textureManager = nullptr;
	AudioManager* audioManager = nullptr;
	InputManager* inputManager = nullptr;
	RenderManager* renderManager = nullptr;
	UIManager* uiManager = nullptr;
	ScoreManager* scoreManager = nullptr;

	Map* map = nullptr;
	Camera* camera = nullptr;
	Cursor* cursor = nullptr;
};
