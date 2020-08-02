#pragma once

#include "Utilities/BasicString.h"

class Window
{
public:
	Window() : mWindow(nullptr) { };
	~Window();

	void init(const BasicString& title, int width, int height);

	SDL_Renderer* createRenderer();

	const SDL_Window* get() const { return mWindow; }
	VectorF size() const;

	void setTitle(const char* title);


private:
	SDL_Window* mWindow;
};