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

	void saveToFile(std::ofstream& file);

	void parseXML(const BasicString& filePath);
	
	rapidxml::xml_document<>& getXML() { return xmlFile; }

	XMLNode rootNode() const;
	XMLNode rootChild(const BasicString& label) const;

	void test()
	{
		xmlFile.first_node()->first_node("Tutorial")->value("OFF");
	}

#if _DEBUG
	BasicString path;
#endif

private:
	rapidxml::file<>* file;
	rapidxml::xml_document<> xmlFile;
};
