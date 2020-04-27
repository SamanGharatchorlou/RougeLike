#pragma once

class XMLParser;

// --- Reads the character stats from a config file ---
class StatReader
{
public:
	XMLValueMap getStats(FileManager::Folder folder, const std::string& config);
};
