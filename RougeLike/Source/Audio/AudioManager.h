#pragma once

#include "SoundController.h"





class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void init();
	void slowUpdate();

	Audio* getAudio(const std::string& label) const;

	void playMusic(const std::string& label);
	void playSound(const std::string& label, void* sourceId);

	void pause(const std::string& label, void* sourceId);
	void resume(const std::string& label, void* sourceId);
	void stop(const std::string& label, void* sourceId);


	bool isPlaying(const std::string& label, void* sourced);

	// audio control
	void setVolume(int theVolume);

	void pauseMusic() const;
	void toggleMute() const;


private:
	int loadAllMusic(FileManager::Folder folder);
	int loadAllSound(FileManager::Folder folder);
	int loadAllSoundGroups(FileManager::Folder folder);

	bool loadAudio(Audio* audio, const std::string& name, const std::string& filePath);

private:
	SoundController mSoundController;

	std::unordered_map<std::string, Audio*> mAudioBank;

	int volume;
};