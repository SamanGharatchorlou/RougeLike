#pragma once



class Audio
{
public:
	virtual ~Audio() { };
	virtual bool load(const BasicString& filePath) = 0;

	virtual void play(int channel) = 0;
	virtual void pause(int channel) { };
	virtual void stop(int channel) { };
};



class Sound : public Audio
{
public:
	Sound() : mChunk(nullptr) { }
	~Sound();

	bool load(const BasicString& filePath) override;

	void play(int channel) override;

private:
	Mix_Chunk *mChunk;
};



class Music : public Audio
{
public:
	Music() : mMusic(nullptr) { }
	~Music();

	bool load(const BasicString& filePath) override;

	void play(int channel) override;
	void pause(int channel) override;
	void stop(int channel) override;

private:
	Mix_Music *mMusic;
};



class AudioGroup : public Audio
{
public:
	AudioGroup() { }
	~AudioGroup();

	bool load(const BasicString& directoryPath) override;

	void play(int channel) override;

private:
	std::vector<Audio*> group;
};