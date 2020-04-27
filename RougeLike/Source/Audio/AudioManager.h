#pragma once

class Audio;

class AudioManager
{
public:
	AudioManager();
	~AudioManager() { };

	void init();

	// load music or clips
	void loadWav(const std::string& name, const std::string& filePath);
	void loadMUS(const std::string& name, const std::string& filePath);

	Audio* getAudio(const std::string& label) const;

	// audio control
	void setVolume(int theVolume);

	void pauseMusic() const;
	void toggleMute() const ;

private:

	std::unordered_map<std::string, Audio*> AudioClips;

	int volume;
};