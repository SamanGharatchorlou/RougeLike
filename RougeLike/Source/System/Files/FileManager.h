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
		Image_Enemies,
		Image_Player,
		Image_Maps,
		Image_Weapons,

		Font,

		map,

		Configs,
		Config_UI,
		Config_Map,
		Config_Animations,
		Config_Enemies,
		Config_Player,
		Config_Weapons,

		Audio,
		Audio_Music,
		Audio_Sound,
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

	std::vector<std::string> allFilesInFolder(const Folder folder) const;

private:
	void addFilesToList(std::vector<std::string>& fileList, const fs::path& directoryPath) const;

private:
	FileManager();
	~FileManager() {};

	std::string folderPaths[count];
};