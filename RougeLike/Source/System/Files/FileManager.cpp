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
	folderPaths[None] = ".";
	folderPaths[Root] = pathToString(fs::current_path()) + "\\Resources\\";

	folderPaths[PreLoadFiles] = folderPaths[Root] + "PreLoadFiles\\";

	// Images
	folderPaths[Images] = folderPaths[Root] + "Images\\";
	folderPaths[Image_UI] = folderPaths[Images] + "UI\\";
	folderPaths[Image_Maps] = folderPaths[Images] + "Maps\\";
	folderPaths[Image_Weapons] = folderPaths[Images] + "Weapons\\";
	folderPaths[Image_Animations] = folderPaths[Images] + "Animations\\";

	// Audio
	folderPaths[Audio] = folderPaths[Root] + "Audio\\";
	folderPaths[Audio_Music] = folderPaths[Audio] + "Music\\";
	folderPaths[Audio_Sound] = folderPaths[Audio] + "Sound\\";
	folderPaths[Audio_SoundGroups] = folderPaths[Audio] + "SoundGroups\\";

	// Font
	folderPaths[Font] = folderPaths[Root] + "Font\\";

	// Configs
	folderPaths[Configs] = folderPaths[Root] + "Configs\\";

	folderPaths[Config_Map] = folderPaths[Configs] + "Map\\";
	folderPaths[Config_Menus] = folderPaths[Configs] + "UIMenus\\";
	folderPaths[Configs_Objects] = folderPaths[Configs] + "Objects\\";

	folderPaths[Configs_MapObjects] = folderPaths[Configs_Objects] + "Map\\";
	folderPaths[Config_Enemies] = folderPaths[Configs_Objects] + "Enemies\\";
	folderPaths[Config_Player] = folderPaths[Configs_Objects] + "Player\\";
	folderPaths[Config_Abilities] = folderPaths[Configs_Objects] + "Abilities\\";

	folderPaths[Config_Weapons] = folderPaths[Configs_Objects] + "Weapons\\";
	folderPaths[Config_MeleeWeapons] = folderPaths[Configs_Objects] + "Weapons\\Melee\\";

	for (int i = 0; i < Folder::Count; i++)
	{
		ASSERT(Warning, !folderPaths[(Folder)i].empty(), "The enum %d in the folderPath map has not been defined\n", i);
	}
}

void FileManager::free()
{
	folderPaths.clear();
}


FileManager::Folder FileManager::getFolderIndex(const BasicString& directory)
{
	for (int i = 0; i < Folder::Count; i++)
	{
		if (directory + "\\" == folderPaths[(Folder)i].c_str())
			return static_cast<Folder>(i);
	}

	DebugPrint(Warning, "Folder path '%s' is not in the folder array\n", directory.c_str());
	return Folder::None;
}


BasicString FileManager::generatePath(const Folder folder) const
{
	BasicString buffer;

	if (folder < Folder::Count)
	{
		buffer = folderPaths.at(folder).c_str();
	}
	else
	{
		DebugPrint(Warning, "No folder found with folder enum %d\n", folder);
		buffer.clear();
	}

	return buffer;
}

fs::path FileManager::fsPath(const Folder folder) const
{
	BasicString buffer;

	if (folder < Folder::Count)
	{
		buffer = folderPaths.at(folder).c_str();
	}
	else
	{
		DebugPrint(Warning, "No folder found with folder enum %d\n", folder);
		buffer.clear();
	}

	return fs::path(buffer.c_str());
}


bool FileManager::exists(const Folder folder, const BasicString& name) const
{
	BasicString outPath("");

	for (const auto& directoryPath : fs::directory_iterator(fsPath(folder)))
	{
		if (!fs::is_directory(directoryPath) && getItemName(directoryPath.path()) == name)
		{
			return true;
		}
		// if directory, search all sub folders
		else if (fs::is_directory(directoryPath)) 
		{
			BasicString outPath("");
			outFilePath(outPath, directoryPath.path(), name);

		}

		if (!outPath.empty())
			return true;
	}

	return false;
}

BasicString FileManager::findFile(const Folder folder, const BasicString& name) const
{
	BasicString outPath("");

	for (const auto& directoryPath : fs::directory_iterator(fsPath(folder)))
	{
		if (!fs::is_directory(directoryPath) && getItemName(directoryPath.path()) == name)
		{
			outPath = pathToString(directoryPath.path());
		}
		else if (fs::is_directory(directoryPath))
		{
			outFilePath(outPath, directoryPath.path(), name);
		}

		if (!outPath.empty())
			return outPath;
	}

	DebugPrint(Warning, "No file named '%s' was found in the folder '%s'\n", name.c_str(), generatePath(folder).c_str());
	return outPath;
}


BasicString FileManager::getItemName(const BasicString& filePath) const
{
	char fileName[50];
	errno_t error = _splitpath_s(filePath.c_str(), NULL, 0, NULL, 0, fileName, 50, NULL, 0);
	return BasicString(fileName);
}


BasicString FileManager::getItemName(const fs::path& filePath) const
{
	char fileName[50];
	errno_t error = _splitpath_s(pathToString(filePath).c_str(), NULL, 0, NULL, 0, fileName, 50, NULL, 0);
	return BasicString(fileName);
}


std::vector<BasicString> FileManager::fullPathsInFolder(const Folder folder) const
{
	std::vector<BasicString> fileNameList;

	for (const auto& fullFilePath : fs::directory_iterator(fsPath(folder)))
	{
		fileNameList.push_back(pathToString(fullFilePath.path()));
	}

	return fileNameList;
}

std::vector<BasicString> FileManager::fullPathsInFolder(const BasicString& directoryPath) const
{
	std::vector<BasicString> fileNameList;

	for (const auto& fullFilePath : fs::directory_iterator(directoryPath.c_str()))
	{
		fileNameList.push_back(pathToString(fullFilePath.path()));
	}

	return fileNameList;
}


// TODO: will also get folder names?
std::vector<BasicString> FileManager::fileNamesInFolder(const Folder folder) const
{
	std::vector<BasicString> fileNameList;

	for (const auto& fullFilePath : fs::directory_iterator(fsPath(folder)))
	{
		fileNameList.push_back(getItemName(fullFilePath.path()));
	}

	return fileNameList;
}


std::vector<BasicString> FileManager::allFilesInFolder(const Folder folder) const
{
	std::vector<BasicString> fileNameList;

	for (const auto& path : fs::directory_iterator(fsPath(folder)))
	{
		if (fs::is_directory(path))
			addFilesToList(fileNameList, path.path());
		else
			fileNameList.push_back(pathToString(path.path()));
	}

	return fileNameList;
}

std::vector<BasicString> FileManager::allFilesInFolder(const fs::path& directoryPath) const
{
	std::vector<BasicString> fileNameList;

	for (const auto& path : fs::directory_iterator(directoryPath))
	{
		if (fs::is_directory(path))
			addFilesToList(fileNameList, path.path());
		else
			fileNameList.push_back(pathToString(path.path()));
	}

	return fileNameList;
}


std::vector<BasicString> FileManager::foldersInFolder(const Folder folder) const
{
	std::vector<BasicString> folderPathsList;

	for (const auto& path : fs::directory_iterator(fsPath(folder)))
	{
		if (fs::is_directory(path))
		{
			folderPathsList.push_back(pathToString(path.path()));
		}
	}

	return folderPathsList;
}


// --- Private Functions --- //
void FileManager::addFilesToList(std::vector<BasicString>& fileList, const fs::path& directoryPath) const
{
	for (const auto& path : fs::directory_iterator(directoryPath))
	{
		if (fs::is_directory(path))
			addFilesToList(fileList, path);
		else
			fileList.push_back(pathToString(path.path()));
	}
}

void FileManager::outFilePath(BasicString& outValue, const fs::path& directoryPath, const BasicString& name) const
{
	for (const auto& dirPath : fs::directory_iterator(directoryPath))
	{
		if (!fs::is_directory(dirPath) && getItemName(dirPath.path()) == name)
		{
			outValue = pathToString(dirPath.path());
		}
		else if (fs::is_directory(dirPath))
		{
			outFilePath(outValue, dirPath.path(), name);
		}

		// End recursion
		if (!outValue.empty())
			return;
	}
}


void FileManager::outFolderPath(BasicString& outValue, const fs::path& directoryPath, const BasicString& name) const
{
	for (const auto& path : fs::directory_iterator(directoryPath))
	{
		if (fs::is_directory(path))
		{
			if (getItemName(path.path()) == name)
			{
				outValue = pathToString(path.path());
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


BasicString pathToString(const fs::path& path)
{
	return BasicString(path.string().c_str());
}