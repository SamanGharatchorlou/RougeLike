#pragma once

class Renderer
{
public:

	static Renderer* Get();
	void free() { SDL_DestroyRenderer(mRenderer); }

	Renderer* create(SDL_Renderer* renderer) 
	{ 
		mRenderer = renderer; 
		return this;
	}

	SDL_Renderer* sdlRenderer() const { return mRenderer; }

private:
	Renderer() { }
	~Renderer() { }

	SDL_Renderer* mRenderer;
};