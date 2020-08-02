#include "pch.h"
#include "Managers/GameController.h"

#include "Debug/MemTrack.h"


int main(int argc, char* args[])
{
	GameController GameApp;

	GameApp.load();
	GameApp.run();
	GameApp.free();

	return 0;
}


