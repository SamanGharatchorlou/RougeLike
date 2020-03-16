#pragma once

struct GameData;
class Map;

class MapLevel
{
public:
	MapLevel(GameData* gameData);

	void GenerateRandomLevel(int width, int height);

	void renderA();
	void renderB();

	void renderExit();

	Map* map(VectorF position) const;
	Map* map() const { return mMap; }
	std::vector<Map*> maps() const;

	VectorF size() const;

private:
	GameData* mGameData;
	Map* mMap;
	Map* mExitTunnel;
};