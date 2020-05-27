#pragma once

class Renderer
{
public:

	static Renderer* Get();

	// Only used while loading for thread safety
	void Open();
	void Close();

	void free() { SDL_DestroyRenderer(mRenderer); }



	Renderer* create(SDL_Renderer* renderer) 
	{ 
		mRenderer = renderer; 
		return this;
	}

	SDL_Renderer* sdlRenderer() const { return mRenderer; }

private:
	Renderer() { mLoadingSemaphor = SDL_CreateSemaphore(1); }
	~Renderer() { }

	SDL_Renderer* mRenderer;

	// For loading only
	SDL_sem* mLoadingSemaphor;
};