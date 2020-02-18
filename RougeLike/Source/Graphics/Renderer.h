#pragma once

class Renderer
{
public:
	~Renderer() { SDL_DestroyRenderer(mRenderer); }

	static Renderer* Get();

	void create(SDL_Renderer* renderer) { mRenderer = renderer; }

	SDL_Renderer* renderer() const { return mRenderer; }

private:
	SDL_Renderer* mRenderer;
};