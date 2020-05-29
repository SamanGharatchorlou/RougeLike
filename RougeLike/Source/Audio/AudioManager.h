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

	// Playback
	void playMusic(const std::string& label);
	void playSound(const std::string& label, void* sourceId);

	void pause(const std::string& label, void* sourceId);
	void resume(const std::string& label, void* sourceId);
	void stop(const std::string& label, void* sourceId);

	bool isPlaying(const std::string& label, void* sourced);

	// volume
	void setSoundVolume(float volume);
	float soundVolume() const;

	void setMusicVolume(float volume);
	float musicVolume() const;

	void toggleMute();
	void mute(bool shouldMute);


private:
	int loadAllMusic(FileManager::Folder folder);
	int loadAllSound(FileManager::Folder folder);
	int loadAllSoundGroups(FileManager::Folder folder);

	bool loadAudio(Audio* audio, const std::string& name, const std::string& filePath);

private:
	SoundController mSoundController;

	std::unordered_map<std::string, Audio*> mAudioBank;
};