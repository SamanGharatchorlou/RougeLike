#pragma once

class Window
{
public:
	Window() : mWindow(nullptr) { };
	~Window();

	void init(const char* title, int width, int height);

	SDL_Renderer* createRenderer();

	const SDL_Window* get() const { return mWindow; }
	VectorF size() const { return mSize; }

	void setTitle(const char* title);


private:
	SDL_Window* mWindow;
	VectorF mSize;
};