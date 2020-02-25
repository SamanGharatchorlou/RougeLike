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
	folderPaths[None]					= std::string("");
	folderPaths[Root_Folder]			= std::string("\\Resources\\");

	folderPaths[Image_UI]				= std::string("Images\\UI\\");
	folderPaths[Image_Enemies]			= std::string("Images\\Enemies\\");
	folderPaths[Image_Character]		= std::string("Images\\Character\\");
	folderPaths[Image_Maps]				= std::string("Images\\Maps\\");
	folderPaths[Image_Weapons]			= std::string("Images\\Weapons\\");

	folderPaths[Font]					= std::string("Font\\");

	folderPaths[Configs]				= std::string("Configs\\");
	folderPaths[Config_UI]				= std::string("Configs\\UIMenus\\");
	folderPaths[Config_Map]				= std::string("Configs\\Map\\");
	folderPaths[Config_Animations]		= std::string("Configs\\Animations\\");
	folderPaths[Config_Weapons]			= std::string("Configs\\Weapons\\");
	folderPaths[Config_Stats_Enemies]	= std::string("Configs\\Stats\\Enemies\\");
	folderPaths[Config_Stats_Player]	= std::string("Configs\\Stats\\Player\\");
}


std::string FileManager::getFolderPath(Folder folder) const
{
	std::string buffer = "empty";

	if (folder < count)
	{
		buffer = fs::current_path().string() +
			folderPaths[Root_Folder] +
			folderPaths[folder];
	}
	else
	{
		DebugPrint(Warning, "No folder found with folder enum %d\n", folder);
		buffer.clear();
	}

	return buffer;
}


// Get filepath with folder specified 
std::string FileManager::getFilePath(Folder folder, std::string fileName) const
{
	std::string buffer = "empty";

	if (folder < count)
	{
		buffer = fs::current_path().string() + 
			folderPaths[Root_Folder] + 
			folderPaths[folder] + 
			fileName;
	}
	else
	{
		DebugPrint(Warning, "No folder found with folder enum %d\n", folder);
		buffer.clear();
	}

	return buffer;
}


// Get filepath with folder specified and xml extension added
std::string FileManager::getXMLFilePath(Folder folder, std::string fileName) const
{
	std::string buffer = "empty";

	if (folder < count)
	{
		buffer = fs::current_path().string() +
			folderPaths[Root_Folder] +
			folderPaths[folder] +
			fileName;
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

	buffer.append(".xml");

	return buffer;
}


bool FileManager::readFile(Folder folder, std::string fileName, std::string& outBuffer)
{
	fs::path filePath = fs::path(getFilePath(folder, fileName));
	
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


std::string FileManager::getFileName(std::string filePath) const
{
	char fileName[50];

	errno_t error = _splitpath_s(filePath.c_str(), NULL, 0, NULL, 0, fileName, 50, NULL, 0);

	return std::string(fileName);
}