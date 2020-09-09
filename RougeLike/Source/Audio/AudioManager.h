#pragma once

#include "SoundController.h"


class AudioManager
{
public:
	static AudioManager* Get();

	void resetController() { mSoundController.clear(); }
	void init();
	void load();
	void unload();

	void preLoad();

	void setSource(Actor* listener, float attenuationDistance);
	void slowUpdate();

	Audio* getAudio(const BasicString& label) const;
	BasicString getLabel(Audio* audio) const;

	// Controls
	void play(const BasicString& label, const void* sourceId, VectorF source = VectorF(-1.0f, -1.0f));
	void loop(const BasicString& label, const void* sourceId, VectorF source = VectorF(-1.0f, -1.0f));

	void pause(const BasicString& label, const void* sourceId);
	void resume(const BasicString& label, const void* sourceId);
	void stop(const BasicString& label, const void* sourceId);

	void fadeIn(const BasicString& label, const void* sourceId, int ms, VectorF source = VectorF(-1.0f, -1.0f));
	void fadeOut(const BasicString& label, const void* sourceId, int ms);

	bool isPlaying(const BasicString& label, const void* sourced);
	bool isActive(const BasicString& label, const void* sourced);

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

	AudioManager();
	~AudioManager();


private:
	SoundController mSoundController;

	std::unordered_map<BasicString, Audio*> mAudioBank;
};