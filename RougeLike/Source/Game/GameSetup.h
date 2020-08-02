#pragma once

#include "Utilities/BasicString.h"

class Window;

class GameSetup
{
public:
	GameSetup();
	Window* initSDL();
	void closeSubSystems();

private:
	void readSettings();

	Window* createWindow();
	bool initRenderer(Window* window);

	void initIMG();
	void initFont();
	void initAudio();


private:
	BasicString title;
	VectorF screenSize;
	int audioChannels;
};

