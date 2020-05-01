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
	folderPaths[None]						= std::string("");
	folderPaths[Root_Folder]				= std::string("\\Resources\\");

	// Images
	folderPaths[Image]						= std::string("Images\\");

	// Audio
	folderPaths[Audio_Music]				= std::string("Audio\\Music\\");
	folderPaths[Audio_Sound]				= std::string("Audio\\Sound\\");

	// Font
	folderPaths[Font]						= std::string("Font\\");

	// Configs
	folderPaths[Configs]					= std::string("Configs\\");
	folderPaths[Config_UI]					= std::string("Configs\\UIMenus\\");
	folderPaths[Config_Map]					= std::string("Configs\\Map\\");
	folderPaths[Config_Animations]			= std::string("Configs\\Animations\\");
	folderPaths[Config_Enemies]				= std::string("Configs\\Objects\\Enemies\\");
	folderPaths[Config_Player]				= std::string("Configs\\Objects\\Player\\");
	folderPaths[Config_Weapons]				= std::string("Configs\\Objects\\Weapons\\");
}


std::string FileManager::folderPath(const Folder folder) const
{
	std::string buffer;

	if (folder < count)
	{
		buffer = std::string(
			fs::current_path().string() +
			folderPaths[Root_Folder] +
			folderPaths[folder]
		);
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

	if (folder < count)
	{
		buffer = std::string(
			fs::current_path().string() + 
			folderPaths[Root_Folder] + 
			folderPaths[folder] + 
			fileName
		);
	}
	else
	{
		DebugPrint(Warning, "No folder found with folder enum %d\n", folder);
		buffer.clear();
	}

	return buffer;
}


// Get filepath with folder specified and xml extension added
std::string FileManager::XMLFilePath(const Folder folder, const std::string& fileName) const
{
	std::string buffer;

	if (folder < count)
	{
		buffer = std::string(
			fs::current_path().string() +
			folderPaths[Root_Folder] +
			folderPaths[folder] +
			fileName
		);
	}
	else
	{
		DebugPrint(Warning, "No folder found with folder enum %d\n", folder);
		buffer.clear();
	}


#if _DEBUG
	if (buffer.find(".xml") != std::string::npos)
	{
		DebugPrint(Log, "The string %s already has the .xml extension, extention was not added\n", buffer);
		return buffer;
	}
#endif

	return buffer.append(".xml");
}


std::string FileManager::fileName(const std::string& filePath) const
{
	char fileName[50];

	errno_t error = _splitpath_s(filePath.c_str(), NULL, 0, NULL, 0, fileName, 50, NULL, 0);

	return std::string(fileName);
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


std::vector<std::string> FileManager::fileNamesInFolder(const Folder folder) const
{
	std::vector<std::string> fileNameList;

	for (const auto& fullFilePath : fs::directory_iterator(this->folderPath(folder)))
	{
		fileNameList.push_back(fileName(fullFilePath.path().string()));
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
