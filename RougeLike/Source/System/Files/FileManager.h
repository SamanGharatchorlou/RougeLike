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
		Config_Map,
		Config_Menus,
		Configs_Objects,
		Configs_MapObjects,
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
	// TODO: replace constructor code with init function
	static FileManager* Get();
	void free();

	BasicString generatePath(const Folder folder) const;

	BasicString findFile(const Folder folder, const BasicString& name) const;

	std::vector<BasicString> fullPathsInFolder(const Folder folder) const;
	std::vector<BasicString> fullPathsInFolder(const BasicString& directory) const;
	std::vector<BasicString> fileNamesInFolder(const Folder folder) const;

	BasicString getItemName(const BasicString& filePath) const;
	BasicString getItemName(const fs::path& filePath) const;

	std::vector<BasicString> allFilesInFolder(const Folder folder) const;
	std::vector<BasicString> allFilesInFolder(const fs::path& directory) const;
	std::vector<BasicString> foldersInFolder(const Folder folder) const;

	Folder getFolderIndex(const BasicString& directory);


private:
	fs::path fsPath(const Folder folder) const;

	void addFilesToList(std::vector<BasicString>& fileList, const fs::path& directoryPath) const;

	void outFolderPath(BasicString& outValue, const fs::path& directoryPath, const BasicString& name) const;
	void outFilePath(BasicString& outValue, const fs::path& directoryPath, const BasicString& name) const;


private:
	FileManager();
	~FileManager() {};


private:
	std::unordered_map<Folder, BasicString> folderPaths;
};

BasicString pathToString(const fs::path& path);