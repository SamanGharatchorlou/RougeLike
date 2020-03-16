#pragma once

class Window;
class TextureManager;
class AudioManager;
class InputManager;
class RenderManager;
class UIManager;
class Map;
class MapLevel;
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

	MapLevel* level = nullptr;
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


	void map(Map* map) { mMap = map; }

	const VectorF mapDimentions() const; // { return mMap->size(); }

	bool isWall(VectorF position) const;

private:
	GameInfo() : mMap(nullptr)  { };
	~GameInfo() { };

	const Map* mMap ;
};
