#pragma once

#include "SoundController.h"

//class Audio;

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
	void stop(const std::string& label, void* sourceId);

	bool isPlaying(const std::string& label, void* sourced);

	// audio control
	void setVolume(int theVolume);

	void pauseMusic() const;
	void toggleMute() const;


private:
	int loadAllMusic(FileManager::Folder folder);
	int loadAllSound(FileManager::Folder folder);

	bool loadAudio(Audio* audio, const std::string& name, const std::string& filePath);

private:
	std::unordered_map<std::string, Audio*> mAudioBank;

	SoundController mSoundController;

	int volume;
};