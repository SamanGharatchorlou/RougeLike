#pragma once

#include "Utilities/BasicString.h"

class Window
{
public:
	Window() : mWindow(nullptr) { };
	~Window();

	void init(const BasicString& title, Vector2D<int> size);

	SDL_Renderer* createRenderer();

	const SDL_Window* get() const { return mWindow; }
	Vector2D<int> size() const;

	void setTitle(const char* title);


private:
	SDL_Window* mWindow;
};