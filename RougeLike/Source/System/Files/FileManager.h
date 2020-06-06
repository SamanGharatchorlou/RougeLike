#pragma once


class FileManager
{
public:
	enum Folder
	{
		None,
		Root,

		PreLoadFiles,

		Images,
		Image_UI,
		Image_Maps,
		Image_Weapons,
		Image_Animations,

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
		Config_Abilities,

		Audio,
		Audio_Music,
		Audio_Sound,
		Audio_SoundGroups,

		Count
	};


public:
	static FileManager* Get();

	std::string generatePath(const Folder folder) const;

	std::string findFolder(const Folder folder, const std::string& name) const;
	std::string findFile(const Folder folder, const std::string& name) const;

	std::vector<std::string> fullPathsInFolder(const Folder folder) const;
	std::vector<std::string> fullPathsInFolder(const std::string& directory) const;
	std::vector<std::string> fileNamesInFolder(const Folder folder) const;

	int fileCount(const std::string& directoryPath) const;

	std::string getItemName(const std::string& filePath) const;

	std::vector<std::string> allFilesInFolder(const Folder folder) const;
	std::vector<std::string> allFilesInFolder(const fs::path& directory) const;
	std::vector<std::string> foldersInFolder(const Folder folder) const;

	Folder getFolderIndex(const std::string& directory);


private:
	void addFilesToList(std::vector<std::string>& fileList, const fs::path& directoryPath) const;

	void outFolderPath(std::string& outValue, const std::string& directoryPath, const std::string& name) const;
	void outFilePath(std::string& outValue, const std::string& directoryPath, const std::string& name) const;

private:
	FileManager();
	~FileManager() {};

	std::string folderPaths[Count];
};