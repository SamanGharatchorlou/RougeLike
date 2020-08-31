#include "pch.h"
#include "Renderer.h"

#include "Game/Camera/Camera.h"


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


void Renderer::setScale(float scale)
{
	SDL_RenderSetScale(mRenderer, scale, scale);
	Camera::Get()->setScale(scale);
}