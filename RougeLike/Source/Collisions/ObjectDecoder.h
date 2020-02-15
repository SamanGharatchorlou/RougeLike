#pragma once

#include "System/Files/XMLParser.h"

struct Wall;

class ObjectDecoder
{
public:
	ObjectDecoder(std::vector<Wall*>& walls) : mWalls(walls) { }
	~ObjectDecoder() { }

	void decodeWalls(std::string levelDataFilePath);

private: 
	std::vector<Wall*>& mWalls;
	XMLParser xmlParser;
};