#include "pch.h"
#include "XMLParser.h"


void XMLParser::parseXML(const BasicString& filePath)
{
	ASSERT(fs::exists(filePath.c_str()), "File path %s does not exist, cannot parse xml file\n", filePath.c_str());

	file = new rapidxml::file<>(filePath.c_str());
	xmlFile.parse<0>(file->data());

#if _DEBUG
	path = filePath;
#endif
}


XMLParser::~XMLParser()
{
	delete file;
}


XMLNode XMLParser::rootNode() const
{
	return XMLNode(xmlFile.first_node());
}


XMLNode XMLParser::rootChild(const BasicString& label) const
{
#if _DEBUG
	XMLNode child = rootNode().child(label);
	if (child.isEmpty())
		DebugPrint(Log, "Parser at path '%s' has no child node labeled '%s'\n", path.c_str(), label.c_str()); // TODO: does this need to be path.c_str()?
	return child;
#endif

	return XMLNode(xmlFile.first_node()->first_node(label.c_str()));
}
