#include "pch.h"
#include "AudioManager.h"
#include "Audio.h"

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
void AudioManager::toggleMute() const
{
	if (Mix_VolumeMusic(-1) == 0 && Mix_Volume(-1, -1) == 0)
	{
		Mix_VolumeMusic(volume);
		Mix_Volume(-1, volume);
		DebugPrint(Log, "Audio ON\n");
	}
	else
	{
		Mix_VolumeMusic(0);
		Mix_Volume(-1, 0);
		DebugPrint(Log, "Audio OFF\n");
	}
}

void AudioManager::pauseMusic() const
{
	//for (auto iter = mAudioBank.begin(); iter != mAudioBank.end(); iter++)
	//{
	//	if (iter->second->isMusic)
	//	{
	//		iter->second->pause();
	//		break;
	//	}
	//}
}


void AudioManager::setVolume(int theVolume)
{
	volume = clamp(theVolume, 0, MIX_MAX_VOLUME);
	Mix_VolumeMusic(volume);
	Mix_Volume(-1, volume);
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
		fails += !loadAudio(audio, FileManager::Get()->fileName(path), path);
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
		fails += !loadAudio(audio, FileManager::Get()->fileName(path), path);
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
		fails += !loadAudio(soundGroup, FileManager::Get()->fileName(path), path);
	}

	return fails;
}


bool AudioManager::loadAudio(Audio* audio, const std::string& name, const std::string& filePath)
{
	if (audio->load(filePath))
	{
		mAudioBank[name] = audio;
		DebugPrint(Log, "Successfully loaded audio '%s' at %s\n", name.c_str(), filePath.c_str());
		return true;
	}
	else
	{
		DebugPrint(Warning, "Failure: audio NOT loaded '%s' at %s\n. SDL_mixer Error: %s\n", name.c_str(), filePath.c_str(), Mix_GetError());
		return false;
	}
}

