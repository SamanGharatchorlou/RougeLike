#pragma once

union uMixer
{
	Mix_Music *gMusic;
	Mix_Chunk *gScratch;
};

class Audio
{
public:
	virtual bool load(const std::string& filePath) = 0;

	virtual void play(int channel) = 0;
	virtual void pause(int channel) { };
	virtual void stop(int channel) { };
};



class Sound : public Audio
{
public:
	bool load(const std::string& filePath) override;

	void play(int channel);

private:

	Mix_Chunk *mChunk;
};


class Music : public Audio
{
public:
	bool load(const std::string& filePath) override;

	void play(int channel);
	void pause(int channel);
	void stop(int channel);

private:

	Mix_Music *mMusic;
};