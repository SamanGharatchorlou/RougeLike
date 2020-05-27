#include "pch.h"
#include "Renderer.h"

Renderer* Renderer::Get()
{
	static Renderer sInstance;
	return &sInstance;
}

void Renderer::Open()
{
	SDL_SemWait(mLoadingSemaphor);
}

void Renderer::Close()
{
	SDL_SemPost(mLoadingSemaphor);
}