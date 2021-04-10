#include "pch.h"
#include "Renderer.h"

#include "Game/Camera/Camera.h"



Renderer::Renderer() : mRenderer(nullptr), mLoadingLock(NULL) { }

Renderer* Renderer::Get()
{
	static Renderer sInstance;
	return &sInstance;
}


void Renderer::lock()
{
	SDL_AtomicLock(&mLoadingLock);
	//SDL_SemWait(mLoadingSemaphor);

	//mMutex.lock();
}


void Renderer::unlock()
{
	SDL_AtomicUnlock(&mLoadingLock);
	//SDL_SemPost(mLoadingSemaphor);

	//mMutex.unlock();
}


void Renderer::setScale(float scale)
{
	SDL_RenderSetScale(mRenderer, scale, scale);
	Camera::Get()->setScale(scale);
}



//SDL_Renderer* Renderer::requestSDLRenderer()
//{
//
//}