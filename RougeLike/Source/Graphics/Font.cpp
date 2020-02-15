#include "pch.h"
#include "Font.h"

Font::Font(SDL_Renderer* newRenderer) : renderer(newRenderer)
{
	texture = nullptr;
	originalDimentions = VectorF();
}


Font::~Font()
{
	if (texture)
		SDL_DestroyTexture(texture);

	TTF_CloseFont(gFont);

	DebugPrint(Log, "font destoryed\n");
}

bool Font::loadFromFile(const std::string font)
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont(font.c_str(), 28);
	if (gFont == nullptr)
	{
		printf("Failed to load font at '%s'! SDL_ttf Error: %s\n", font.c_str(), TTF_GetError());
		success = false;
	}

	return success;
}


// TODO: dont need half of this!!!
// TODO: dont need half of this!!!
bool Font::setText(std::string textureText, SDL_Color textColor)
{
	// final texture
	SDL_Texture* tempTexture = nullptr;

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);

	if (!textSurface)
	{
		DebugPrint(Warning, "Unable to render text surface for text: %s! SDL_ttf Error: %s\n", textureText.c_str(), TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		tempTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		if (tempTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			originalDimentions = VectorF(static_cast<float>(textSurface->w), static_cast<float>(textSurface->h));
		}

		// loaded surface no longer needed
		SDL_FreeSurface(textSurface);
	}

	// return sucess
	texture = tempTexture;
	return texture != nullptr;
}

void Font::render(const Rect<int> rect) const
{
	SDL_Rect renderQuad = { (rect.x1), (rect.y1),
							(rect.Width()), (rect.Height()) };

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}

void Font::render(const Rect<int> rect, SDL_RendererFlip flip) const
{
	SDL_Rect renderQuad = { (rect.x1), (rect.y1),
							(rect.Width()), (rect.Height()) };

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, 0.0, NULL, flip);
}


void Font::render(const RectF rect) const
{
	SDL_Rect renderQuad = { static_cast<int>(rect.x1),
							static_cast<int>(rect.y1),
							static_cast<int>(rect.Width()),
							static_cast<int>(rect.Height()) };

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}


// Renders texture with the roation specified
void Font::render(const RectF rect, double rotation, VectorF aboutPoint) const
{
	// texture being displayed on the screen
	SDL_Rect renderQuad = { static_cast<int>(rect.x1),
						static_cast<int>(rect.y1),
						static_cast<int>(rect.Width()),
						static_cast<int>(rect.Height()) };

	// rotate about this point
	SDL_Point point = { aboutPoint.x, aboutPoint.y };

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, rotation, &point, SDL_FLIP_NONE);
}