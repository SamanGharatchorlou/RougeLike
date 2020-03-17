#pragma once

struct GameData;
class Map;

class MapLevel
{
public:
	MapLevel(GameData* gameData);

	void generateNextLevel();
	void generateExitTunnel();

	void swapToEntrance();
	void swapToExit();

	void renderA();
	void renderB();

	Map* map(VectorF position = VectorF(-1.0f, -1.0f)) const;

	VectorF size() const;

	bool entraceOutOfView() const;
	bool mapOutOfView() const;

	bool hasExit() const { return mExitTunnel; }
	bool hasEntrance() const { return mEntraceTunnel; }

private:
	void generateRandomLevel(int width, int height, float offset);

private:
	GameData* mGameData;

	Map* mEntraceTunnel;
	Map* mMap;
	Map* mExitTunnel;

	Vector2D<int> mapSize;
};