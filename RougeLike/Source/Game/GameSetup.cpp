#include "pch.h"
#include "GameSetup.h"

#include "System/Window.h"

GameSetup::GameSetup() : title(""), audioChannels(0) 
{
	// random seed
	srand((unsigned int)time(NULL));
}


Window* GameSetup::initSDL()
{
	Window* window = nullptr;
	readSettings();

	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0)
	{
		window = createWindow();

		if (window)
		{
			bool success = initRenderer(window);
			if (success)
			{
				initIMG();
				initFont();
				initAudio();
				SDL_ShowCursor(false);
			}
			else
				DebugPrint(Error, "Renderer could not be created! SDL Image Error: %s\n", IMG_GetError());
		}
		else
			DebugPrint(Error, "Window could not be created! SDL Error: %s\n", SDL_GetError());
	}
	else
		DebugPrint(Error, "SDL could not be initialised! SDL_Error: %s\n", SDL_GetError());

	return window;
}


// --- Private Functions --- //

void GameSetup::initIMG()
{
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		DebugPrint(Error, "SDL Image was not initialised! SDL Image Error: %s\n", IMG_GetError());
	}
}

void GameSetup::initFont()
{
	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		DebugPrint(Error, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}
}
void GameSetup::initAudio()
{
	// init audio mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		DebugPrint(Error, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
}

void GameSetup::closeSubSystems()
{
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}


Window* GameSetup::createWindow()
{
	Window* window = new Window;
	window->init(title, screenSize);
	return window;
}

bool GameSetup::initRenderer(Window* window)
{
	SDL_Renderer* sdlRenderer = window->createRenderer();
	Renderer::Get()->create(sdlRenderer);
	return sdlRenderer != nullptr;
}


void GameSetup::readSettings()
{
	XMLParser parser(FileManager::Get()->findFile(FileManager::Configs, "GameSettings"));
	StringMap settings(parser.rootChild("Game"));

	title = settings["Title"];
	int width = settings.getInt("Width");
	int height = settings.getInt("Height");
	screenSize = Vector2D<int>(width, height);
	audioChannels = settings.getInt("AudioChannels");
}