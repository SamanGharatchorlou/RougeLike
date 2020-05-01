#pragma once

class Window;
class TextureManager;
class AudioManager;
class InputManager;
class RenderManager;
class UIManager;
class Environment;
class Cursor;
class PlayerManager;
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
	Cursor* cursor = nullptr;

	PlayerManager* playerManager = nullptr;
	EnemyManager* enemies = nullptr;
};


// Read only game info
struct GameInfo
{
public:
	// TODO: having a const version doesnt work???
	//static const GameInfo* Get()
	//{
	//	static GameInfo instance;
	//	return &instance;
	//}

	// Only use in GameData when initing the data
	static GameInfo* Get()
	{
		static GameInfo instance;
		return &instance;
	}


	void map(Environment* map) { mMap = map; }

	const VectorF mapDimentions() const;

	bool isWall(VectorF position) const;

private:
	GameInfo() : mMap(nullptr)  { };
	~GameInfo() { };

	const Environment* mMap ;
};
