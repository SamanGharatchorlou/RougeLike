#include "pch.h"
#include "Font.h"


Font::~Font()
{
	if (mTexture)
		SDL_DestroyTexture(mTexture);

	if(mFont)
		TTF_CloseFont(mFont);

	DebugPrint(Log, "font destoryed\n");
}


bool Font::loadFromFile(const std::string& font, int ptSize)
{
	//Loading success flag
	bool success = true;

	//Open the font
	mFont = TTF_OpenFont(font.c_str(), ptSize);
	if (mFont == nullptr)
	{
		printf("Failed to load font at '%s'! SDL_ttf Error: %s\n", font.c_str(), TTF_GetError());
		success = false;
	}

	// TODO: can I find a way to not save this, it's a large string
	// save a copy of the font name
	mFontName = std::string(font);

	mPtSize = ptSize;

	return success;
}


void Font::resize(int ptSize)
{
	// Font must be reloaded
	TTF_CloseFont(mFont);
	mFont = TTF_OpenFont(mFontName.c_str(), ptSize);
	mPtSize = ptSize;
}


void Font::setText(const std::string& text)
{
	if (mFont != nullptr)
	{
		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, text.c_str(), colour);

		if (!textSurface)
		{
			DebugPrint(Warning, "Unable to render text surface for text: %s! SDL_ttf Error: %s\n", text.c_str(), TTF_GetError());
		}
		else
		{
			if (mTexture)
				SDL_DestroyTexture(mTexture);

			//Create texture from surface pixels
			mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);

			if (mTexture == nullptr)
			{
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				size = VectorF(static_cast<float>(textSurface->w), static_cast<float>(textSurface->h));
			}

			// loaded surface no longer needed
			SDL_FreeSurface(textSurface);
			textSurface = nullptr;
		}
	}
	else
	{
		DebugPrint(Error, "Font has not beed loaded for text: %s, Call Font::loadFromFile first\n", text);
		return;
	}
}


void Font::render(const VectorF position) const
{
	SDL_Rect renderQuad = { static_cast<int>(position.x),
							static_cast<int>(position.y),
							static_cast<int>(size.x),
							static_cast<int>(size.y) };

	SDL_RenderCopyEx(mRenderer, mTexture, nullptr, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}
