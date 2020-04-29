#pragma once

class Audio;

class AudioManager
{
public:
	AudioManager();
	~AudioManager() { };

	void init();

	// load music or clips
	int loadAllMUS(FileManager::Folder folder);
	int loadAllWAV(FileManager::Folder folder);

	bool loadWav(const std::string& name, const std::string& filePath);
	bool loadMUS(const std::string& name, const std::string& filePath);

	Audio* getAudio(const std::string& label) const;

	bool isPlaying(const std::string& label) const;
	void play(const std::string& label);
	void stop(const std::string& label);

	// audio control
	void setVolume(int theVolume);

	void pauseMusic() const;
	void toggleMute() const ;

private:

	std::unordered_map<std::string, Audio*> AudioClips;

	int volume;
};