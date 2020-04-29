#pragma once


class FileManager
{
public:
	enum Folder
	{
		None,
		Root_Folder,

		Image_UI,
		Image_Characters_Enemies,
		Image_Characters_Player,
		Image_Maps,
		Image_Weapons,

		Font,

		map,

		Configs,
		Config_UI,
		Config_Map,
		Config_Animations,
		Config_Stats_Enemies,
		Config_Stats_Player,
		Config_Stats_Weapons,

		Audio,
		Audio_Bg,
		Audio_Effects,
		Audio_Effects_Attack,
		Audio_Effects_GetHit,
		Audio_Effects_Physics,

		count
	};


public:
	static FileManager* Get();

	std::string folderPath(const Folder folder) const;
	std::string filePath(const Folder folder, const std::string& fileName) const;
	std::string XMLFilePath(const Folder folder, const std::string& fileName) const;

	std::vector<std::string> fullPathsInFolder(const Folder folder) const;
	std::vector<std::string> fileNamesInFolder(const Folder folder) const;

	bool readFile(const Folder folder, const std::string& fileName, std::string& outBuffer);

	std::string fileName(const std::string& filePath) const;

private:
	FileManager();
	~FileManager() {};

	std::string folderPaths[count];
};