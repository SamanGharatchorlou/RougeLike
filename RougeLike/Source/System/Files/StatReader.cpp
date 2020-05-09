#include "pch.h"
#include "StatReader.h"


ValueMap StatReader::getStats(FileManager::Folder folder, const std::string& config)
{
	std::string configFilePath = FileManager::Get()->filePath(folder, config);

	XMLParser parser;
	parser.parseXML(configFilePath);

	ValueMap valueMap;
	std::string name = "";
	float value = 0.0f;

	xmlNode rootNode = parser.rootNode();
	xmlNode node = rootNode->first_node();

	while (node != nullptr)
	{
		name = node->name();
		value = std::stof(node->value());
		valueMap[name] = value;

		node = node->next_sibling();
	}

#if _DEBUG
	int statCounter = 0;
	xmlNode statNode = rootNode->first_node();
	while (statNode != nullptr)
	{
		statCounter++;
		statNode = statNode->next_sibling();
	}

	if (statCounter != valueMap.size())
	{
		DebugPrint(Log, "%d stats from the %s config file have not been added to the map\n", 
			statCounter - valueMap.size(), config.c_str());
	}
#endif

	return valueMap;
}
