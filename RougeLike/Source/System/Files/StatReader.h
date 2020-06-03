#pragma once


using StatMap = std::unordered_map<std::string, float>;

// --- Reads the character stats from a config file ---
class StatReader
{
public:
	StatMap getStats(FileManager::Folder folder, const std::string& config);
};
