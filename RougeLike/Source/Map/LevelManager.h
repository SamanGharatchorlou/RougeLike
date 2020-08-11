#pragma once

#include "TileTypes.h"

class Map;
class TextureManager;

class LevelManager
{
public:
	LevelManager(TextureManager* textureManger);
	~LevelManager();

	void load(const XMLParser& parser);

	void slowUpdate(float dt);
	void renderLowDepth();
	void renderHeighDepth();

	void buildLevel(const XMLParser& parser);

	int level() const { return mLevel; }
	void incrementLevel() { mLevel++; }

	Map* map(VectorF position) const;
	const Map* entrance() const { return mMaps.entrance; }
	const Map* primaryMap() const { return mMaps.primaryMap; }
	const Map* exit() const { return mMaps.exit; }

	VectorF size() const;


private:
	// Building maps
	void createNewMaps();

	void buildPrimanyAndExit(const XMLParser& parser);
	void buildMap(const XMLNode dataNode, Map* map, VectorF offset);

	void swapEntranceExit();

	// Reading map data
	void fillData(const XMLNode sectionNode, Map* map);
	DecorMap readDecorData(const XMLNode& root) const;
	void readConfigData(Vector2D<int>& mapIndexSize, VectorF& tileSize, float& scale);

	void setTrapInfo(Map* map, DecorMap& trapInfo);

	VectorF getOffset(const Map* map) const;
	
private:
	struct Maps
	{
		Maps() : entrance(nullptr), primaryMap(nullptr), exit(nullptr) { }
		Map* entrance;
		Map* primaryMap;
		Map* exit;
	} mMaps;



	TextureManager* mTextureManager;
	int mLevel;

};