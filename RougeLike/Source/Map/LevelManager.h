#pragma once

#include "TileTypes.h"

class Map;
class TextureManager;

class LevelManager
{
public:
	LevelManager(TextureManager* textureManger) : mTextureManager(textureManger), mLevel(0) { }

	void init(Map* entrance);

	void buildPrimary(Map* map, VectorF offset);
	void buildExit(Map* map, VectorF offset);

	void swapEntranceExit(Map*& entrance, Map*& exit);

	int level() const { return mLevel; }
	void incrementLevel() { mLevel++; }

	VectorF getOffset(const Map* map) const;


private:
	void decorate(Map* map, const std::string& section);
	DecorMap getLevelDecorInfo(XMLParser& parser, const std::string& section);

	void setTrapInfo(Map* map, DecorMap& trapInfo);
	DecorMap getLevelTrapInfo(XMLParser& parser, const std::string& section);


private:
	TextureManager* mTextureManager;
	int mLevel;

};