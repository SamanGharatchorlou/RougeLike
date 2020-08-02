#include "pch.h"
#include "Window.h"

void Window::init(const BasicString& title, int width, int height)
{
	// create window
	mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN);
}


Window::~Window()
{
	if(mWindow)
		SDL_DestroyWindow(mWindow);
	
	mWindow = nullptr;
}


SDL_Renderer* Window::createRenderer()
{
	 return SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED );
}


void Window::setTitle(const char* title)
{
	SDL_SetWindowTitle(mWindow, title);
}


VectorF Window::size() const
{
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(mWindow, &width, &height);
	return VectorF(width, height);
}