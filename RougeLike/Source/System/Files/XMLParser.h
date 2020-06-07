#pragma once
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

typedef rapidxml::xml_node<>* xmlNode;
typedef rapidxml::xml_attribute<>* xmlAttributes;
using ValueMap = std::unordered_map<std::string, std::string>;

class XMLParser
{

public:
	XMLParser() { };
	XMLParser(const std::string& file) { parseXML(file); }

	~XMLParser()
	{
		delete file;
		file = nullptr;
	}

	void parseXML(const std::string& filePath);

	rapidxml::xml_document<>& getXML() { return xmlFile; }

	xmlNode rootNode() const;

	std::string nodeValue(xmlNode node, const std::string& label);

	Attributes attributes(xmlNode root) const;

	ValueMap values(xmlNode node) const;

	std::string firstRootNodeValue(const std::string& label) const;

#if _DEBUG
	std::string path;
#endif

private:
	rapidxml::file<>* file;
	rapidxml::xml_document<> xmlFile;
};

