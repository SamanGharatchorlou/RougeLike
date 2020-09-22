#include "pch.h"
#include "Game/GameController.h"

//
#pragma comment(lib, "SDL2_image")


int main(int argc, char* args[])
{
	GameController GameApp;

	GameApp.init();
	GameApp.preLoad();
	GameApp.load();

	GameApp.run();

	GameApp.free();

	PRINT_MEMORY;

	int a = 4;

	return 0;
}


