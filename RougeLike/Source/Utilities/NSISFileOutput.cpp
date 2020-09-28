#include "pch.h"
#include "NSISFileOutput.h"


void OutputNSISFolderInfo()
{
	FileManager* fm = FileManager::Get();

	std::unordered_set<BasicString> allFolders;
	fm->AllFoldersContainingFiles(fs::path(fm->generatePath(FileManager::Root).c_str()), allFolders);

	BasicString cwdPath("C:\\Users\\Saman\\RougeLike\\RougeLike\\Resources\\");
	int cwdLength = cwdPath.length();

	BasicString installDir("${GAME_FILES}");
	BasicString resource("${RESOURCES_ROOT}");

	std::vector<BasicString> defines;
	std::vector<BasicString> outFolders;

	float counter = 0;
	std::unordered_set<BasicString>::const_iterator setIter;
	for (setIter = allFolders.begin(); setIter != allFolders.end(); setIter++)
	{
		BasicString folderName = fm->getItemName(*setIter);
		BasicString subbed = setIter->substr(cwdLength, setIter->length() - cwdLength);
		BasicString formatted = resource + "\\" + subbed;

		defines.push_back("FOLDER_" + folderName + "_" + BasicString(counter));
		outFolders.push_back(formatted);

		counter += 1.0f;
	}

	// Print Defines
	for (int i = 0; i < outFolders.size(); i++)
	{
		// !define FOLDER_music "${RESOURCES_ROOT}\Audio\Music"
		DebugPrint(Log, "  !define %s \"%s\"\n", defines[i].c_str(), outFolders[i].c_str());
	}
	DebugPrint(Log, "\n\n");

	// SetOutpaths
	for (int i = 0; i < outFolders.size(); i++)
	{	
		// SetOutPath "$INSTDIR\${FOLDER_MUSIC}"
		// File / r "${FOLDER_MUSIC}\*.*"
		DebugPrint(Log, "  SetOutPath \"%s\\${%s}\"\n", installDir.c_str(), defines[i].c_str());
		DebugPrint(Log, "  File /r \"${%s}\\*.*\"\n", defines[i].c_str());
	}
	DebugPrint(Log, "\n\n");


	// Delete all files
	std::vector<BasicString> allFiles = fm->allFilesInFolder(FileManager::Root);
	for (int i = 0; i < allFiles.size(); i++)
	{
		BasicString subbed = allFiles[i].substr(cwdLength, allFiles[i].length() - cwdLength);

		// Delete "$INSTDIR\${RESOURCES_ROOT}\PreLoadFiles\LoadingBar.png"
		DebugPrint(Log, "  Delete \"%s\\%s\\%s\"\n", installDir.c_str(), resource.c_str(), subbed.c_str());
	}
	DebugPrint(Log, "\n\n");


	std::unordered_set<BasicString> allGameFolders;
	fm->AllFolders(fs::path(fm->generatePath(FileManager::Root).c_str()), allGameFolders);

	std::list<BasicString> list;
	for (setIter = allGameFolders.begin(); setIter != allGameFolders.end(); setIter++)
	{
		list.push_back(*setIter);
	}
	list.sort(compare_length);

	std::list<BasicString>::const_iterator listIter;
	for (listIter = list.begin(); listIter != list.end(); listIter++)
	{
		BasicString subbed = listIter->substr(cwdLength, listIter->length() - cwdLength);

		// RMDir "$INSTDIR\${FOLDER_MUSIC}"
		DebugPrint(Log, "  RMDir \"%s\\%s\\%s\"\n", installDir.c_str(), resource.c_str(), subbed.c_str());
	}
	DebugPrint(Log, "\n\n");
}


bool compare_length(const BasicString& first, const BasicString& second)
{
	return first.length() > second.length();
}