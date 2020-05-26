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
	folderPaths[Root] = std::string(fs::current_path().string() + "\\Resources\\");

	// Images
	folderPaths[Image] = std::string("Images\\");
	folderPaths[Image_UI] = std::string(folderPaths[Image] + "UI\\");
	folderPaths[Image_Maps] = std::string(folderPaths[Image] + "Maps\\");
	folderPaths[Image_Weapons] = std::string(folderPaths[Image] + "Weapons\\");
	folderPaths[Image_Characters] = std::string(folderPaths[Image] + "Characters\\");
	folderPaths[Image_Effects] = std::string(folderPaths[Image] + "Effects\\");

	folderPaths[Image_END] = std::string(folderPaths[Image] + "Dummy");

	// Audio
	folderPaths[Audio_Music] = std::string("Audio\\Music\\");
	folderPaths[Audio_Sound] = std::string("Audio\\Sound\\");
	folderPaths[Audio_SoundGroups] = std::string("Audio\\SoundGroups\\");

	// Font
	folderPaths[Font] = std::string("Font\\");

	// Configs
	folderPaths[Configs] = std::string("Configs\\");
	folderPaths[Configs_Objects] = std::string(folderPaths[Configs] + "Objects\\");

	folderPaths[Config_UI] = std::string(folderPaths[Configs] + "UIMenus\\");
	folderPaths[Config_Map] = std::string(folderPaths[Configs] + "Map\\");
	folderPaths[Config_Animations] = std::string(folderPaths[Configs] + "Animations\\");
	folderPaths[Config_Enemies] = std::string(folderPaths[Configs_Objects] + "Enemies\\");
	folderPaths[Config_Player] = std::string(folderPaths[Configs_Objects] + "Player\\");
	folderPaths[Config_Weapons] = std::string(folderPaths[Configs_Objects] + "Weapons\\");
	folderPaths[Config_MeleeWeapons] = std::string(folderPaths[Configs_Objects] + "Weapons\\Melee\\");

	for (int i = 0; i < Folder::Count; i++)
	{
		ASSERT(Warning, !folderPaths[i].empty(), "The enum %d in the folderPath map has not been defined\n", i);
	}
}


std::string FileManager::folderPath(const Folder folder) const
{
	std::string buffer;

	if (folder < Folder::Count)
	{
		buffer = std::string(folderPaths[Root] + folderPaths[folder]);
	}
	else
	{
		DebugPrint(Warning, "No folder found with folder enum %d\n", folder);
		buffer.clear();
	}

	return buffer;
}


// Get filepath with folder specified 
std::string FileManager::filePath(const Folder folder, const std::string& fileName) const
{
	std::string buffer;

	if (folder < Folder::Count)
	{
		buffer = std::string(folderPaths[Root] + folderPaths[folder] + fileName);
	}
	else
	{
		DebugPrint(Warning, "No folder found with folder enum %d\n", folder);
		buffer.clear();
	}

	return buffer;
}


std::string FileManager::filePath(const std::string& directoryPath, const std::string& itemName) const
{
	fs::path dirPath = fs::path(directoryPath);
	if (!fs::is_directory(dirPath))
		DebugPrint(Warning, "Item at path '%s' is not a directoy. Cannot search for file '%s'.\n", directoryPath.c_str(), itemName.c_str());
	else
	{
		for (const auto& filePath : fs::directory_iterator(dirPath))
		{
			std::string path = filePath.path().string();
			if (getFileName(path) == itemName)
			{
				return path;
			}
		}

		DebugPrint(Warning, "No file with name '%s' contained within directory '%s'\n", itemName.c_str(), directoryPath.c_str());
	}

	return std::string("");
}


std::string FileManager::findFileInFolder(const Folder folder, const std::string& fileName) const
{
	std::vector<std::string> fileList = allFilesInFolder(folder);

	for (int i = 0; i < fileList.size(); i++)
	{
		if (getFileName(fileList[i]) == fileName)
		{
			return fileList[i];
		}
	}

	DebugPrint(Warning, "The file '%s' could not be found within the directory '%s'\n", fileName.c_str(), folderPath(folder).c_str());
	return "";
}


// Get filepath with folder specified and xml extension added
std::string FileManager::XMLFilePath(const Folder folder, const std::string& fileName) const
{
	std::string buffer;

	if (folder < Folder::Count)
	{
		buffer = std::string(folderPaths[Root] + folderPaths[folder] + fileName);
	}
	else
	{
		DebugPrint(Warning, "No folder found with folder enum %d\n", folder);
		buffer.clear();
	}


#if _DEBUG
	if (buffer.find(".xml") != std::string::npos)
	{
		DebugPrint(Log, "The string %s already has the .xml extension, extention was not added\n", buffer.c_str());
		return buffer;
	}
#endif

	return buffer.append(".xml");
}


std::string FileManager::getFileName(const std::string& filePath) const
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

	for (const auto& fullFilePath : fs::directory_iterator(this->folderPath(folder)))
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


std::vector<std::string> FileManager::fileNamesInFolder(const Folder folder) const
{
	std::vector<std::string> fileNameList;

	for (const auto& fullFilePath : fs::directory_iterator(this->folderPath(folder)))
	{
		fileNameList.push_back(getFileName(fullFilePath.path().string()));
	}

	return fileNameList;
}


std::vector<std::string> FileManager::allFilesInFolder(const Folder folder) const
{
	std::vector<std::string> fileNameList;

	for (const auto& path : fs::directory_iterator(this->folderPath(folder)))
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

	for (const auto& path : fs::directory_iterator(this->folderPath(folder)))
	{
		if (fs::is_directory(path))
		{
			folderPathsList.push_back(path.path().string());
		}
	}

	return folderPathsList;
}


// Recursivly finds all the file names
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


bool FileManager::readFile(const Folder folder, const std::string& fileName, std::string& outBuffer)
{
	std::string filePathString = filePath(folder, fileName);
	fs::path filePath = fs::path(filePathString);
	
	if (fs::exists(filePath))
	{
		outBuffer.clear();
		std::ifstream ifs(filePath.string().c_str(), std::ios::in | std::ios::binary | std::ios::ate);

		ifs.seekg(0, std::ios::end);
		outBuffer.reserve((size_t)ifs.tellg());
		ifs.seekg(0, std::ios::beg);

		outBuffer.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		return true;
	}
	else
		return false;

}
