#pragma once

union uMixer
{
	Mix_Music *gMusic;
	Mix_Chunk *gScratch;
};

class Audio
{
public:
	virtual ~Audio() { };
	virtual bool load(const std::string& filePath) = 0;

	virtual void play(int channel) = 0;
	virtual void pause(int channel) { };
	virtual void stop(int channel) { };
};



class Sound : public Audio
{
public:
	Sound() : mChunk(nullptr) { }
	~Sound();

	bool load(const std::string& filePath) override;

	void play(int channel) override;

private:

	Mix_Chunk *mChunk;
};


class Music : public Audio
{
public:
	Music() : mMusic(nullptr) { }
	~Music();

	bool load(const std::string& filePath) override;

	void play(int channel) override;
	void pause(int channel) override;
	void stop(int channel) override;

private:

	Mix_Music *mMusic;
};