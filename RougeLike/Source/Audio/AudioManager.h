#pragma once

#include "SoundController.h"

struct AudioEvent
{
	enum Action
	{
		Play,
		Loop,
		Pause,
		Resume,
		Stop,
		FadeIn,
		FadeOut,
	};

	AudioEvent(Action _action, const BasicString _label, const void* _id)								: action(_action), label(_label), id(reinterpret_cast<const uintptr_t>(_id)), source(VectorF(-1.0f, -1.0f)), time(-1) { }
	AudioEvent(Action _action, const BasicString _label, const void* _id, VectorF _source)				: action(_action), label(_label), id(reinterpret_cast<const uintptr_t>(_id)), source(_source),				 time(-1) { }
	AudioEvent(Action _action, const BasicString _label, const void* _id, int _time)					: action(_action), label(_label), id(reinterpret_cast<const uintptr_t>(_id)), source(VectorF(-1.0f, -1.0f)), time(_time) { }
	AudioEvent(Action _action, const BasicString _label, const void* _id, VectorF _source, int _time)	: action(_action), label(_label), id(reinterpret_cast<const uintptr_t>(_id)), source(_source),				 time(_time) { }

	const Action action;
	const BasicString label;
	const uintptr_t id;
	const VectorF source;
	const int time;

	Audio* audio;
};




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
	void handleEvents();

	Audio* getAudio(const BasicString& label) const;
	BasicString getLabel(Audio* audio) const;

	void pushEvent(AudioEvent event);

	bool isPlaying(const BasicString& label, const void* sourced) const;
	bool isActive(const BasicString& label, const void* sourced) const;

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



	Queue<AudioEvent> mEvents;
};