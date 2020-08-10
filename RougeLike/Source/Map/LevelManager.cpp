#include "pch.h"
#include "LevelManager.h"

#include "TileTypes.h"
#include "Map.h"

#include "MapBuilding/MapGenerator.h"
#include "MapBuilding/MapDecorator.h"

#include "Game/Camera.h"


LevelManager::LevelManager(TextureManager* textureManger) : mTextureManager(textureManger), mLevel(0) { }

LevelManager::~LevelManager()
{
	delete mMaps.entrance;
	delete mMaps.primaryMap;
	delete mMaps.exit;
}


void LevelManager::load(const XMLParser& parser)
{
	createNewMaps();

	buildEntrance();
	buildPrimary(parser);
	buildExit(parser);
}


void LevelManager::buildLevel(const XMLParser& parser)
{
	swapEntranceExit();

	buildPrimary(parser);
	buildExit(parser);
}


void LevelManager::slowUpdate(float dt)
{
	mMaps.entrance->slowUpdate(dt);
	mMaps.primaryMap->slowUpdate(dt);
	mMaps.exit->slowUpdate(dt);
}


void LevelManager::renderLowDepth()
{
	mMaps.entrance->renderFloor();
	mMaps.primaryMap->renderFloor();
	mMaps.exit->renderFloor();

	mMaps.entrance->renderLowerLayer();
	mMaps.primaryMap->renderLowerLayer();
	mMaps.exit->renderLowerLayer();
}


void LevelManager::renderHeighDepth()
{
	mMaps.entrance->renderUpperLayer();
	mMaps.primaryMap->renderUpperLayer();
	mMaps.exit->renderUpperLayer();
}


VectorF LevelManager::size() const
{
	return mMaps.primaryMap->size();
}


Map* LevelManager::map(VectorF position) const
{
	if (position.x < mMaps.primaryMap->getFirstRect().LeftPoint())
		return mMaps.entrance;
	else if (position.x < mMaps.primaryMap->getLastRect().RightPoint())
		return mMaps.primaryMap;
	else
		return mMaps.exit;
}


// --- Private Functions --- //

// Building
void LevelManager::createNewMaps()
{
	Vector2D<int> primaryMapSize;
	VectorF tileSize;
	float scale = -1.0f;
	readConfigData(primaryMapSize, tileSize, scale);

	tileSize *= scale;

	mMaps.primaryMap = new Map(primaryMapSize, tileSize);

	Vector2D<int> corridorMapSize;
	corridorMapSize.x = (int)((Camera::Get()->size().x / mMaps.primaryMap->tileSize().x) * 1.5f); // TODO: replace with config data
	corridorMapSize.y = primaryMapSize.y;

	mMaps.entrance = new Map(corridorMapSize, tileSize);
	mMaps.exit = new Map(corridorMapSize, tileSize);
}


void LevelManager::buildEntrance()
{
	Map* map = mMaps.entrance;
	map->clearData();
	MapGenerator generator;
	generator.buildCorridor(map->getData());
	map->populateData(mTextureManager, VectorF(0, 0));
}


void LevelManager::buildPrimary(const XMLParser& parser)
{
	Map* map = mMaps.primaryMap;
	map->clearData();

	MapGenerator generator;
	generator.buildDungeon(map->getData());

	XMLNode primaryNode = parser.rootChild("Primary");
	readMapData(primaryNode, map);

	VectorF offset = getOffset(mMaps.entrance);
	map->populateData(mTextureManager, offset);
}


void LevelManager::buildExit(const XMLParser& parser)
{
	Map* map = mMaps.exit;
	map->clearData();

	MapGenerator generator;
	generator.buildCorridor(map->getData());

	XMLNode exitNode = parser.rootChild("Exit");
	readMapData(exitNode, map);

	VectorF offset = getOffset(mMaps.primaryMap);
	map->populateData(mTextureManager, offset);
}


void LevelManager::swapEntranceExit()
{
	Map* oldEntrace = mMaps.entrance;
	mMaps.entrance = mMaps.exit;
	mMaps.exit = oldEntrace;
}


VectorF LevelManager::getOffset(const Map* map) const
{
	return VectorF(map->getLastRect().RightPoint(), 0.0f);
}

// Reading data
void LevelManager::readConfigData(Vector2D<int>& mapIndexSize, VectorF& tileSize, float& scale)
{
	XMLParser parser;
	BasicString path = FileManager::Get()->findFile(FileManager::Config_Map, "Map");
	parser.parseXML(path);

	XMLNode tileSetInfoNode = parser.rootChild("TilesetInfo");

	// Map size
	XMLNode tileCountNode = tileSetInfoNode.child("TileCount");
	DataMap<BasicString> tileCountData = tileCountNode.nodeAttributes();

	int tileCountX = tileCountData.getInt("x");
	int tileCountY = tileCountData.getInt("y");
	mapIndexSize.set(tileCountX, tileCountY);

	// Tile size
	XMLNode tileSizeNode = tileSetInfoNode.child("TileSize");
	DataMap<BasicString> tileSizeData = tileSizeNode.nodeAttributes();

	float tileSizeX = tileSizeData.getFloat("x");
	float tileSizeY = tileSizeData.getFloat("y");
	tileSize.set(tileSizeX, tileSizeY);

	// Scale
	XMLNode scaleNode = parser.rootChild("Scale");
	scale = atof(scaleNode.value().c_str());
}


void LevelManager::readMapData(const XMLNode sectionNode, Map* map)
{
	// Decorations
	XMLNode decorNode = XMLNode(sectionNode.child("Decor"));
	DecorMap decorations = readDecorData(decorNode);
	MapDecorator decorator;
	decorator.addDecor(decorations, map->getData());

	// Traps
	XMLNode trapNode = XMLNode(sectionNode.child("Traps"));
	DecorMap trapInfo = readDecorData(trapNode);
	setTrapInfo(map, trapInfo);
}


DecorMap LevelManager::readDecorData(const XMLNode& root) const
{
	DecorMap decorMap;
	if (root)
	{
		XMLNode item = root.child();
		while (!item.isEmpty())
		{
			DecorType type = stringToType(item.name());
			StringMap attributes = item.nodeAttributes();
			decorMap[type] = attributes;

			item = item.next();
		}
	}

	return decorMap;
}


void LevelManager::setTrapInfo(Map* map, DecorMap& trapInfo)
{
	if (trapInfo.count(DecorType::Spikes))
	{
		const StringMap attributes = trapInfo[DecorType::Spikes];
		if (attributes.contains("triggerTime") && attributes.contains("recoveryTime"))
		{
			Damage damage = attributes.getFloat("damage");
			float triggerTime = attributes.getFloat("triggerTime");
			float recoveryTime = attributes.getFloat("recoveryTime");
			map->traps().set(damage, triggerTime, recoveryTime);
		}
	}
}



