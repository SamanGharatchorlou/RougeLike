#pragma once
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

#include "XMLNode.h"

class XMLParser
{
public:
	XMLParser() { };
	XMLParser(const BasicString& file) { parseXML(file); }

	~XMLParser();

	void parseXML(const BasicString& filePath);

	rapidxml::xml_document<>& getXML() { return xmlFile; }

	xmlNode rootNode() const;
	XMLNode root() const;

	BasicString nodeValue(xmlNode node, const BasicString& label) const;

	Attributes attributes(xmlNode root) const;

	StringMap stringMap(xmlNode node) const;
	ValueMap valueMap(xmlNode node) const;

	BasicString firstRootNodeValue(const BasicString& label) const;

#if _DEBUG
	BasicString path;
#endif

private:
	rapidxml::file<>* file;
	rapidxml::xml_document<> xmlFile;
};

