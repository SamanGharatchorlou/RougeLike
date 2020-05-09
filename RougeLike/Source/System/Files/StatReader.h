#pragma once


class XMLParser;

using ValueMap = std::unordered_map<std::string, float>;

// --- Reads the character stats from a config file ---
class StatReader
{
public:
	ValueMap getStats(FileManager::Folder folder, const std::string& config);
};
