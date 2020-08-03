#include "pch.h"
#include "Managers/GameController.h"


int main(int argc, char* args[])
{
	//GameController* GameAppPtr = new GameController;
	GameController GameApp;// = *GameAppPtr;

	GameApp.load();

	GameApp.run();

	GameApp.free();

	PRINT_MEMORY;

	return 0;
}


