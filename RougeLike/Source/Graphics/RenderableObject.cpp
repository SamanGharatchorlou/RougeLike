#include "pch.h"
#include "Renderer.h"

#include "Game/Camera/Camera.h"


Renderer::Renderer() { 
	//mLoadingSemaphor = SDL_CreateSemaphore(1);
	mLoadingLock = NULL;
}

Renderer* Renderer::Get()
{
	static Renderer sInstance;
	return &sInstance;
}


void Renderer::lock()
{
	SDL_AtomicLock(&mLoadingLock);
	//SDL_SemWait(mLoadingSemaphor);
}


void Renderer::unlock()
{
	SDL_AtomicUnlock(&mLoadingLock);
	//SDL_SemPost(mLoadingSemaphor);
}


void Renderer::setScale(float scale)
{
	SDL_RenderSetScale(mRenderer, scale, scale);
	Camera::Get()->setScale(scale);
}