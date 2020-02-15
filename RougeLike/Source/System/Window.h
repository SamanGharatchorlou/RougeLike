#pragma once

class Window
{
public:
	Window(const char* title, float width, float height);
	~Window();

	SDL_Renderer* createRenderer();

	const SDL_Window* get() const { return mWindow; }
	VectorF size() const { return mSize; }

	void setTitle(const char* title);


private:
	SDL_Window* mWindow;
	VectorF mSize;
};