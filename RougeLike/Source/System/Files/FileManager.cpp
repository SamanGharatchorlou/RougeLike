#include "pch.h"
#include "FileManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>


FileManager* FileManager::Get()
{
	static FileManager sInstance;
	return &sInstance;
}


FileManager::FileManager()
{
	folderPaths[None] = std::string(".");
	folderPaths[Root] = std::string(fs::current_path().string() + "\\Resources\\");

	folderPaths[PreLoadFiles] = std::string(folderPaths[Root] + "PreLoadFiles\\");

	// Images
	folderPaths[Images] = std::string(folderPaths[Root] + "Images\\");
	folderPaths[Image_UI] = std::string(folderPaths[Images] + "UI\\");
	folderPaths[Image_Maps] = std::string(folderPaths[Images] + "Maps\\");
	folderPaths[Image_Weapons] = std::string(folderPaths[Images] + "Weapons\\");
	folderPaths[Image_Animations] = std::string(folderPaths[Images] + "Animations\\");

	// Audio
	folderPaths[Audio] = std::string(folderPaths[Root] + "Audio\\");
	folderPaths[Audio_Music] = std::string(folderPaths[Audio] + "Music\\");
	folderPaths[Audio_Sound] = std::string(folderPaths[Audio] + "Sound\\");
	folderPaths[Audio_SoundGroups] = std::string(folderPaths[Audio] + "SoundGroups\\");

	// Font
	folderPaths[Font] = std::string(folderPaths[Root] + "Font\\");

	// Configs
	folderPaths[Configs] = std::string(folderPaths[Root] + "Configs\\");
	folderPaths[Configs_Objects] = std::string(folderPaths[Configs] + "Objects\\");

	folderPaths[Config_UI] = std::string(folderPaths[Configs] + "UIMenus\\");
	folderPaths[Config_Map] = std::string(folderPaths[Configs] + "Map\\");
	folderPaths[Config_Enemies] = std::string(folderPaths[Configs_Objects] + "Enemies\\");
	folderPaths[Config_Player] = std::string(folderPaths[Configs_Objects] + "Player\\");
	folderPaths[Config_Abilities] = std::string(folderPaths[Configs_Objects] + "Abilities\\");

	folderPaths[Config_Weapons] = std::string(folderPaths[Configs_Objects] + "Weapons\\");
	folderPaths[Config_MeleeWeapons] = std::string(folderPaths[Configs_Objects] + "Weapons\\Melee\\");


	for (int i = 0; i < Folder::Count; i++)
	{
		ASSERT(Warning, !folderPaths[i].empty(), "The enum %d in the folderPath map has not been defined\n", i);
	}
}


FileManager::Folder FileManager::getFolderIndex(const std::string& directory)
{
	for (int i = 0; i < Folder::Count; i++)
	{
		if (directory + "\\" == folderPaths[i])
			return static_cast<Folder>(i);
	}

	DebugPrint(Warning, "Folder path '%s' is not in the folder array\n", directory.c_str());
	return Folder::None;
}


std::string FileManager::generatePath(const Folder folder) const
{
	std::string buffer;

	if (folder < Folder::Count)
	{
		buffer = folderPaths[folder];
	}
	else
	{
		DebugPrint(Warning, "No folder found with folder enum %d\n", folder);
		buffer.clear();
	}

	return buffer;
}




std::string FileManager::findFolder(const Folder folder, const std::string& name) const
{
	std::string outPath = "";

	for (const auto& path : fs::directory_iterator(generatePath(folder)))
	{
		if (fs::is_directory(path))
		{
			if (getItemName(path.path().string()) == name)
			{
				return path.path().string();
			}
			else
			{
				outFolderPath(outPath, path.path().string(), name);

				if (!outPath.empty())
					return outPath;
			}
		}
	}

	DebugPrint(Warning, "No folder named '%s' was found in the folder '%s'\n", name.c_str(), generatePath(folder).c_str());
}



std::string FileManager::findFile(const Folder folder, const std::string& name) const
{
	std::string outPath = "";

	for (const auto& path : fs::directory_iterator(generatePath(folder)))
	{
		std::string item = getItemName(path.path().string());
		if (!fs::is_directory(path) && getItemName(path.path().string()) == name)
		{
			outPath = path.path().string();
		}
		else if (fs::is_directory(path))
		{
			outFilePath(outPath, path.path().string(), name);
		}

		if (!outPath.empty())
			return outPath;
	}

	DebugPrint(Warning, "No file named '%s' was found in the folder '%s'\n", name.c_str(), generatePath(folder).c_str());
}


std::string FileManager::getItemName(const std::string& filePath) const
{
	char fileName[50];

	errno_t error = _splitpath_s(filePath.c_str(), NULL, 0, NULL, 0, fileName, 50, NULL, 0);

	return std::string(fileName);
}


int FileManager::fileCount(const std::string& directoryPath) const
{
	int directoryFileCount = 0;

	fs::path dirPath = fs::path(directoryPath);
	if (!fs::is_directory(dirPath))
	{
		DebugPrint(Warning, "Item at path '%s' is not a directoy, file count = 0.\n", directoryPath.c_str());
		return directoryFileCount;
	}

	for (const auto& filePath : fs::directory_iterator(dirPath))
	{
		directoryFileCount++;
	}

	return directoryFileCount;
}


std::vector<std::string> FileManager::fullPathsInFolder(const Folder folder) const
{
	std::vector<std::string> fileNameList;

	for (const auto& fullFilePath : fs::directory_iterator(generatePath(folder)))
	{
		fileNameList.push_back(fullFilePath.path().string());
	}

	return fileNameList;
}

std::vector<std::string> FileManager::fullPathsInFolder(const std::string& directoryPath) const
{
	std::vector<std::string> fileNameList;

	for (const auto& fullFilePath : fs::directory_iterator(directoryPath))
	{
		fileNameList.push_back(fullFilePath.path().string());
	}

	return fileNameList;
}


// TODO: will also get folder names?
std::vector<std::string> FileManager::fileNamesInFolder(const Folder folder) const
{
	std::vector<std::string> fileNameList;

	for (const auto& fullFilePath : fs::directory_iterator(generatePath(folder)))
	{
		fileNameList.push_back(getItemName(fullFilePath.path().string()));
	}

	return fileNameList;
}


std::vector<std::string> FileManager::allFilesInFolder(const Folder folder) const
{
	std::vector<std::string> fileNameList;

	for (const auto& path : fs::directory_iterator(generatePath(folder)))
	{
		if (fs::is_directory(path))
			addFilesToList(fileNameList, path.path());
		else
			fileNameList.push_back(path.path().string());
	}

	return fileNameList;
}

std::vector<std::string> FileManager::allFilesInFolder(const fs::path& directoryPath) const
{
	std::vector<std::string> fileNameList;

	for (const auto& path : fs::directory_iterator(directoryPath))
	{
		if (fs::is_directory(path))
			addFilesToList(fileNameList, path.path());
		else
			fileNameList.push_back(path.path().string());
	}

	return fileNameList;
}


std::vector<std::string> FileManager::foldersInFolder(const Folder folder) const
{
	std::vector<std::string> folderPathsList;

	for (const auto& path : fs::directory_iterator(generatePath(folder)))
	{
		if (fs::is_directory(path))
		{
			folderPathsList.push_back(path.path().string());
		}
	}

	return folderPathsList;
}


// --- Private Functions --- //
void FileManager::addFilesToList(std::vector<std::string>& fileList, const fs::path& directoryPath) const
{
	for (const auto& path : fs::directory_iterator(directoryPath))
	{
		if (fs::is_directory(path))
			addFilesToList(fileList, path);
		else
			fileList.push_back(path.path().string());
	}
}



void FileManager::outFilePath(std::string& outValue, const std::string& directoryPath, const std::string& name) const
{
	for (const auto& path : fs::directory_iterator(directoryPath))
	{
		if (!fs::is_directory(path) && getItemName(path.path().string()) == name)
		{
			outValue = path.path().string();
		}
		else if (fs::is_directory(path))
		{
			outFilePath(outValue, path.path().string(), name);
		}

		// End recursion
		if (!outValue.empty())
			return;
	}
}

void FileManager::outFolderPath(std::string& outValue, const std::string& directoryPath, const std::string& name) const
{
	for (const auto& path : fs::directory_iterator(directoryPath))
	{
		if (fs::is_directory(path))
		{
			if (getItemName(path.path().string()) == name)
			{
				outValue = path.path().string();
			}
			else if (fs::is_directory(path))
			{
				outFolderPath(outValue, path.path().string(), name);
			}
		}

		// End recursion
		if (!outValue.empty())
			return;
	}
}