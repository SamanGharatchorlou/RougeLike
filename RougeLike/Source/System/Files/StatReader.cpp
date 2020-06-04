#include "pch.h"
#include "StatReader.h"


StatMap StatReader::getStats(FileManager::Folder folder, const std::string& config)
{
	std::string configFilePath = FileManager::Get()->findFile(folder, config);

	XMLParser parser;
	parser.parseXML(configFilePath);

	StatMap valueMap;
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

	return valueMap;
}
