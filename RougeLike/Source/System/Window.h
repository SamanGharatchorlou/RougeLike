#pragma once

class Window
{
public:
	Window(const char* title, int width, int height);
	~Window();

	SDL_Renderer* createRenderer();

	const SDL_Window* get() const { return mWindow; }
	VectorF size() const { return mSize; }

	void setTitle(const char* title);


private:
	SDL_Window* mWindow;
	VectorF mSize;
};