#pragma once

#include "Events/Dispatcher.h"


class Map;
class TextureManager;


class Environment : public Dispatcher
{
public:
	Environment(TextureManager* textureManager);

	void init();
	void restart();


	void nextLevel();

	bool canClosePreviousLevel(VectorF playerPosition) const;

	void renderFloor();
	void renderBottomLayer();
	void renderTopLayer();

	Map* map(VectorF position) const;
	Map* primaryMap() const { return mPrimaryMap; }

	VectorF size() const;
	int mapLevel() const { return mMapLevel; }

	void setCameraBoundaries();
	VectorF toWorldCoords(VectorF cameraCoords);


private:
	void createNewMaps();
	void readConfigData(Vector2D<int>& mapIndexSize, VectorF& tileSize, float& scale);

	void buildLevel(float offset);
	void buildPassage(Map* map, float offset);


private:
	TextureManager* mTextureManager;

	Map* mEntrace;
	Map* mPrimaryMap;
	Map* mExit;

	std::vector<Map*> maps;

	Vector2D<int> mMapSize;

	int mMapLevel;
};