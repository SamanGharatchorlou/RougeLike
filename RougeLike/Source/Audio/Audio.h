#pragma once



class Audio
{
public:
	virtual ~Audio() { };
	virtual bool load(const BasicString& filePath) = 0;

	virtual void play(int channel) = 0;
	virtual void pause(int channel) = 0;
	virtual void resume(int channel) = 0;
	virtual void stop(int channel) = 0;

	virtual void playNext(int channel) = 0;
	virtual bool isPlaying(int channel) const = 0;

#if PRINT_PLAY_AUDIO
	BasicString mFilePath;
#endif
};



class Sound : public Audio
{
public:
	Sound() : mChunk(nullptr) { }
	~Sound();

	bool load(const BasicString& filePath) override;

	void play(int channel) override;
	void playNext(int channel) override { play(channel); }

	void resume(int channel) override;
	void pause(int channel) override;
	void stop(int channel) override;

	bool isPlaying(int channel) const override;

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
	void playNext(int channel) override { play(channel); }

	void resume(int channel) override { DebugPrint(Warning, "UNIMPLEMENTED\n"); }
	void pause(int channel) override;
	void stop(int channel) override;

	bool isPlaying(int channel) const override;

private:
	Mix_Music *mMusic;
};



class AudioGroup : public Audio
{
public:
	AudioGroup() : playingIndex(0) { }
	~AudioGroup();

	bool load(const BasicString& directoryPath) override;

	void play(int channel) override;
	void playNext(int channel) override;

	void resume(int channel) override;
	void pause(int channel) override;
	void stop(int channel) override;

	bool isPlaying(int channel) const override;

private:
	std::vector<Audio*> group;
	int playingIndex;
};