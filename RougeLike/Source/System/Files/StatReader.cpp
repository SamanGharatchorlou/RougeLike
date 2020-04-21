#include "pch.h"
#include "StatReader.h"


XMLValueMap StatReader::getStats(FileManager::Folder folder, std::string config)
{
	std::string configFilePath = FileManager::Get()->filePath(folder, config);

	XMLParser parser;
	parser.parseXML(configFilePath);

	XMLValueMap valueMap;
	value stat;
	std::string name;

	xmlNode rootNode = parser.rootNode();
	xmlNode node = rootNode->first_node();

	while (node != nullptr)
	{
		name = node->name();

		Attributes attributes = parser.attributes(node);

		if (attributes.contains("type"))
		{
			std::string type = attributes.getString("type");

			if (strcmp(type.c_str(), "int") == 0)
			{
				stat.i = std::stoi(node->value());
			}
			else if (strcmp(type.c_str(), "float") == 0)
			{
				stat.f = std::stof(node->value());
			}
			else if (strcmp(type.c_str(), "double") == 0)
			{
				stat.d = std::stod(node->value());
			}
			else if (strcmp(type.c_str(), "string") == 0)
			{
				const char* string = node->value();

				ASSERT(Warning, strlen(string) < 25, 
					"The %s attribute of length %d from %s is too big to fit into the union value char. \
					It must be less than 25 characters, either reduce the text size or increase the value union data type\n", 
					name.c_str(), strlen(string), configFilePath.c_str());

				strcpy_s(stat.c, string);
			}
			else
			{
				DebugPrint(Warning, "Unrecognised type %s, no value added\n", type.c_str());

				node = node->next_sibling();
				continue;
			}

			valueMap.add(name, stat);
		}
		else
		{
			DebugPrint(Warning,
				"Attribute %s has no type attribute, no value added\n", name.c_str());
		}

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
