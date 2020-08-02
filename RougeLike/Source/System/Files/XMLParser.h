#pragma once
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

#include "XMLNode.h"


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
	XMLNode root() const;

	std::string nodeValue(xmlNode node, const std::string& label) const;

	Attributes attributes(xmlNode root) const;

	StringMap stringMap(xmlNode node) const;
	ValueMap valueMap(xmlNode node) const;

	std::string firstRootNodeValue(const std::string& label) const;

#if _DEBUG
	std::string path;
#endif

private:
	rapidxml::file<>* file;
	rapidxml::xml_document<> xmlFile;
};

