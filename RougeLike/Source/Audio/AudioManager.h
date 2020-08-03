#pragma once

#include "SoundController.h"


class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void init();
	void slowUpdate();

	Audio* getAudio(const BasicString& label) const;

	// Playback
	void playMusic(const BasicString& label);
	void playSound(const BasicString& label, void* sourceId);

	void pause(const BasicString& label, void* sourceId);
	void resume(const BasicString& label, void* sourceId);
	void stop(const BasicString& label, void* sourceId);

	bool isPlaying(const BasicString& label, void* sourced);

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

	bool loadAudio(Audio* audio, const BasicString& name, const BasicString& filePath);

private:
	SoundController mSoundController;

	std::unordered_map<BasicString, Audio*> mAudioBank;
};