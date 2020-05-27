#include "pch.h"
#include "Managers/GameController.h"

int main(int argc, char* args[])
{
	GameController GameApp("RougeLike Game");
	GameApp.load();
	GameApp.run();
	return 0;
}