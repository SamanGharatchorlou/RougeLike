#pragma once

#include "Utilities/BasicString.h"

class Window;

class GameSetup
{
public:
	GameSetup();

	void initFileSystem();
	Window* initSDLWindow();

	void static closeSubSystems();
	void static setTutorial(const BasicString& mode);

private:
	void readSettings();

	Window* createWindow();
	bool initRenderer(Window* window);

	void initIMG();
	void initFont();
	void initAudio();


private:
	BasicString title;
	Vector2D<int> screenSize;
};

