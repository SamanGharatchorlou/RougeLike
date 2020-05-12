#pragma once


class FileManager
{
public:
	enum Folder
	{
		None,
		Root_Folder,

		Image,
		Image_UI,
		Image_Maps,
		Image_Weapons,
		Image_Characters,
		Image_END,

		Font,

		Configs,
		Configs_Objects,
		Config_UI,
		Config_Map,
		Config_Animations,
		Config_Enemies,
		Config_Player,
		Config_Weapons,
		Config_MeleeWeapons,

		Audio,
		Audio_Music,
		Audio_Sound,
		Audio_SoundGroups,

		count
	};


public:
	static FileManager* Get();

	std::string folderPath(const Folder folder) const;
	std::string filePath(const Folder folder, const std::string& fileName) const;
	std::string filePath(const std::string& directoryPath, const std::string& itemName) const;
	std::string XMLFilePath(const Folder folder, const std::string& fileName) const;

	std::vector<std::string> fullPathsInFolder(const Folder folder) const;
	std::vector<std::string> fullPathsInFolder(const std::string& directoryPath) const;
	std::vector<std::string> fileNamesInFolder(const Folder folder) const;

	int fileCount(const std::string& directoryPath) const;

	bool readFile(const Folder folder, const std::string& fileName, std::string& outBuffer);

	std::string fileName(const std::string& filePath) const;

	std::vector<std::string> allFilesInFolder(const Folder folder) const;
	std::vector<std::string> foldersInFolder(const Folder folder) const;


private:
	void addFilesToList(std::vector<std::string>& fileList, const fs::path& directoryPath) const;

private:
	FileManager();
	~FileManager() {};

	std::string folderPaths[count];
};