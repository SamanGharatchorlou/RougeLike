#pragma once


class FileManager
{
public:
	enum Folder
	{
		None,
		Root_Folder,

		Image_UI,
		Image_Enemies,
		Image_Character,
		Image_Maps,
		Image_Weapons,

		Font,

		map,

		Configs,
		Config_UI,
		Config_Map,
		Config_Animations,
		Config_Weapons,
		Config_Stats,

		count
	};


public:
	static FileManager* Get();

	std::string getFolderPath(Folder folder);
	std::string getFilePath(Folder folder, std::string fileName);
	std::string getXMLFilePath(Folder folder, std::string fileName);

	bool readFile(Folder folder, std::string fileName, std::string& outBuffer);


private:
	FileManager();
	~FileManager() {};

	std::string folderPaths[count];
};