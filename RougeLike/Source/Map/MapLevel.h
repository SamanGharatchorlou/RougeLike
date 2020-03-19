#pragma once

class Map;

class MapLevel
{
public:
	MapLevel();

	void generateNextLevel();
	void generateNextExit();

	void swapToEntrance();
	void swapToExit();

	void renderA(const TextureManager* tm, float depth);
	void renderB(const TextureManager* tm, float depth);

	Map* map(VectorF position = VectorF(-1.0f, -1.0f)) const;

	VectorF size() const;

	bool entraceOutOfView(VectorF position) const;
	bool mapOutOfView(VectorF position) const;

	void generateEntrace(float offset);
	void generateExit(float offset);

	void closeEntrance();
	void closeLevel();


private:
	void generateRandomLevel(int width, int height, float offset);


private:
	Map* mEntrace;
	Map* mMap;
	Map* mExit;

	Vector2D<int> mapSize;
};