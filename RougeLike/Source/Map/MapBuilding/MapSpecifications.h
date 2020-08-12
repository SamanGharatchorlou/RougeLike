#pragma once

#include "Utilities/Maps/DecorMap.h"

class Map;

class MapSpecifications
{
public:
	void set(const XMLNode node);
	const DecorMap& getDecor(const Map* map);
	VectorF size(const BasicString& value) const { return mSizes.at(value); }


private:
	void readTileSize(const XMLNode node);
	void readMapSize(const XMLNode node);
	void fillSpecifications(const XMLNode node);


private:
	std::unordered_map<BasicString, VectorF> mSizes;

	DecorMap mDungeonDecor;
	DecorMap mCorridorDecor;
};