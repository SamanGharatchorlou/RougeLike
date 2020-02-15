#pragma once

class XMLParser;

// --- Reads the character stats from a config file ---
class StatReader
{
public:
	XMLValueMap getStats(std::string config);
};
