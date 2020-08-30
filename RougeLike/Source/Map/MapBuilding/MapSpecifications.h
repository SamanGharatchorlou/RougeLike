#pragma once

#include "Utilities/Maps/DecorMap.h"

class Map;

using TrapDataMap = std::unordered_map<DecorType, PropertyMap>;

class MapSpecifications
{
public:
	void clear();

	void set(const XMLNode node);
	const DecorMap& getDecor(const Map* map);
	VectorF size(const BasicString& value) const { return mSizes.at(value); }

	void readTrapData();
	const TrapDataMap* trapData() { return &mTrapData; }

private:
	void readTileSize(const XMLNode node);
	void readMapSize(const XMLNode node);
	void fillSpecifications(const XMLNode node);


private:
	std::unordered_map<BasicString, VectorF> mSizes;

	DecorMap mDungeonDecor;
	DecorMap mCorridorDecor;

	TrapDataMap mTrapData;
};