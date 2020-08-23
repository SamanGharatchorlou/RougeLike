#include "pch.h"
#include "Window.h"

void Window::init(const BasicString& title, Vector2D<int> size)
{
	// create window
	mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		size.x, size.y, SDL_WINDOW_SHOWN);
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


Vector2D<int> Window::size() const
{
	int width = -1;
	int height = -1;
	SDL_GetWindowSize(mWindow, &width, &height);
	return Vector2D<int>(width, height);
}