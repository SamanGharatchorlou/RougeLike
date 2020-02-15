#pragma once
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

typedef rapidxml::xml_node<>* xmlNode;
typedef rapidxml::xml_attribute<>* xmlAttributes;

class XMLParser
{

public:
	XMLParser() { };
//	XMLParser(FileManager::Folder folder, std::string file) 
//	{ 
//		std::string filePath = FileManager::Get()->getFilePath(folder, file);
//		parseXML(filePath); 
//#if _DEBUG
//		path = filePath;
//#endif
//	}

	~XMLParser()
	{
		delete file;
		file = nullptr;
	}

	void parseXML(std::string filePath)
	{
		// TODO: this assert will not trigger for config/mapS/dungeon.xml i.e. map has extra S so file doesnt exist there
		// but it will pass the assert, does the exists function work correctly?
		ASSERT(fs::exists(filePath), "File path %s does not exist, cannot parse xml file\n", filePath.c_str());

		file = new rapidxml::file<>(filePath.c_str());
		xmlFile.parse<0>(file->data());

#if _DEBUG
		path = filePath;
#endif
	}

	rapidxml::xml_document<>& getXML() { return xmlFile; }

	xmlNode getRootNode();
	Attributes getAttributes(xmlNode root);

	// TODO: implement get all nodes
	// Attributes getAllNodeAttributes(xmlNode root);
	std::string getNodeData(xmlNode dataNode);

#if _DEBUG
	std::string path;
#endif

private:
	rapidxml::file<>* file;
	rapidxml::xml_document<> xmlFile;
};

