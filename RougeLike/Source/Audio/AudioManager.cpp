#include "pch.h"
#include "AudioManager.h"
#include "Audio.h"

#include "Game/LoadingManager.h"

AudioManager::AudioManager()
{
	DebugPrint(Log, "Audio manager created\n");
}


AudioManager::~AudioManager()
{
	for (std::pair<std::string, Audio*> audio : mAudioBank)
	{
		delete audio.second;
		audio.second = nullptr;
		audio.first = "";
	}
}


// -- Audio Loading -- //
void AudioManager::init()
{
	DebugPrint(Log, "\n--- Loading Audio ---\n");
	int fails = 0;

	DebugPrint(Log, "\nBackground Music\n");
	fails += loadAllMusic(FileManager::Audio_Music);

	DebugPrint(Log, "\nAudio Effects\n");
	fails += loadAllSound(FileManager::Audio_Sound);

	DebugPrint(Log, "\nSound Groups\n");
	fails += loadAllSoundGroups(FileManager::Audio_SoundGroups);

	DebugPrint(Log, "\n--- Audio Loading Complete: %d Failures ---\n\n", fails);
}


void AudioManager::slowUpdate()
{
	mSoundController.slowUpdate();
}



Audio* AudioManager::getAudio(const std::string& label) const
{
	auto search = mAudioBank.find(label);

	if (search != mAudioBank.end())
	{
		return search->second;
	}
	else
	{
		DebugPrint(Warning, "No item in audio map with label: %s\n", label.c_str());
		return nullptr;
	}
}


// -- Audio Control -- //
void AudioManager::toggleMute()
{
	mSoundController.toggleMute();
}


void AudioManager::setSoundVolume(float volume)
{
	mSoundController.setSoundVolume(volume);
}

float AudioManager::soundVolume() const
{
	return mSoundController.getSoundVolume();
}

void AudioManager::setMusicVolume(float volume)
{
	mSoundController.setMusicVolume(volume);
}

float AudioManager::musicVolume() const
{
	return mSoundController.getMusicVolume();
}


void AudioManager::playSound(const std::string& label, void* sourceId)
{
	Audio* audio = getAudio(label);

	if (audio)
		mSoundController.playSound(audio, sourceId);
}

void AudioManager::playMusic(const std::string& label)
{
	Audio* audio = getAudio(label);

	if (audio)
		mSoundController.playMusic(audio);
}

void AudioManager::stop(const std::string& label, void* sourceId)
{
	Audio* audio = getAudio(label);

	if (audio)
		mSoundController.stopSound(audio, sourceId);
}

void AudioManager::pause(const std::string& label, void* sourceId)
{
	Audio* audio = getAudio(label);

	if (audio)
		mSoundController.pauseSound(audio, sourceId);
}

void AudioManager::resume(const std::string& label, void* sourceId)
{
	Audio* audio = getAudio(label);

	if (audio)
		mSoundController.resumeSound(audio, sourceId);
}

bool AudioManager::isPlaying(const std::string& label, void* sourceId)
{
	Audio* audio = getAudio(label);

	if (audio)
		 return mSoundController.isPlaying(audio, sourceId);
}



// --- Private Functions --- //
int AudioManager::loadAllMusic(FileManager::Folder folder)
{
	int fails = 0;
	std::vector<std::string> imagePaths = FileManager::Get()->fullPathsInFolder(folder);

	for (const std::string& path : imagePaths)
	{
		Audio *audio = new Music;
		fails += !loadAudio(audio, FileManager::Get()->getFileName(path), path);
	}

	return fails;
}


int AudioManager::loadAllSound(FileManager::Folder folder)
{
	int fails = 0;
	std::vector<std::string> imagePaths = FileManager::Get()->allFilesInFolder(folder);

	for (const std::string& path : imagePaths)
	{
		Audio *audio = new Sound;
		fails += !loadAudio(audio, FileManager::Get()->getFileName(path), path);
	}

	return fails;
}


int AudioManager::loadAllSoundGroups(FileManager::Folder folder)
{
	int fails = 0;
	std::vector<std::string> folderPaths = FileManager::Get()->foldersInFolder(folder);

	for (const std::string& path : folderPaths)
	{
		Audio* soundGroup =  new AudioGroup;
		fails += !loadAudio(soundGroup, FileManager::Get()->getFileName(path), path);
	}

	return fails;
}


bool AudioManager::loadAudio(Audio* audio, const std::string& name, const std::string& filePath)
{
	bool success = true;
	if (audio->load(filePath))
	{
		// Add to has loaded files
		LoadingManager::Get()->successfullyLoaded(filePath);

		mAudioBank[name] = audio;
		DebugPrint(Log, "Successfully loaded audio '%s' at %s\n", name.c_str(), filePath.c_str());
	}
	else
	{
		DebugPrint(Warning, "Failure: audio NOT loaded '%s' at %s\n. SDL_mixer Error: %s\n", name.c_str(), filePath.c_str(), Mix_GetError());
		success = false;
	}

	return success;
}
