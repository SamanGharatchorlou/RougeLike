#pragma once

union uMixer
{
	Mix_Music *gMusic;
	Mix_Chunk *gScratch;
};

class Audio
{
public:
	bool loadWav(const std::string& filePath);
	bool loadMUS(const std::string& filePath);

	void play() const;
	void pause() const;
	void stop() const;

	bool isMusic;

private:

	uMixer mixer;
};