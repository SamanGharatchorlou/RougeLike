#include "pch.h"
#include "Game/GameController.h"


int main(int argc, char* args[])
{
	GameController GameApp;

	GameApp.load();

	GameApp.run();

	GameApp.free();

	PRINT_MEMORY;

	return 0;
}


