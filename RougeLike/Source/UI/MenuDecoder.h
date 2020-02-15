#pragma once

typedef std::vector<std::vector<Attributes>> MenuAttributes;

/*
Decodes an XML formatted file into a UI built into layers, boxes & buttons
Note: The layers must be in order with the top layer first/at the top of the file
*/
class MenuDecoder
{
public:
	MenuDecoder() { };
	~MenuDecoder() { }

	MenuAttributes getMenuAttributes(std::string menuDataFilePath);

private:
	XMLParser xmlParser;
};
