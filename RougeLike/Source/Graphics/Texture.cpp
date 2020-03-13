#include "pch.h"
#include "Texture.h"

Texture::Texture() : texture(nullptr), renderer(nullptr) { }


Texture::~Texture()
{
	free();
	DebugPrint(Log, "Texture destoryed\n");
}


bool Texture::loadFromFile(const std::string& filePath)
{
	// remove any existing texture
	free();

	// final texture
	SDL_Texture* tempTexture = nullptr;

	renderer = Renderer::Get()->sdlRenderer();

	// load image
	SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());

	if (!loadedSurface)
	{
		DebugPrint(Warning, "Unable to load image %s! SDL Image Error: %s\n", filePath.c_str(), IMG_GetError());
	}
	else
	{
		// create texture from surface pixels
		if (!renderer)
		{
			DebugPrint(Warning, "No renderer has been attached to texture: %s\n", filePath.c_str());
		}
		else
		{
			tempTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

			// set image dimentions
			if (!tempTexture)
			{
				DebugPrint(Warning, "Unable to create texture from %s! SDL Error: %s\n", filePath.c_str(), SDL_GetError());
			}
			else
			{
				originalDimentions = VectorF(static_cast<float>(loadedSurface->w), static_cast<float>(loadedSurface->h));
			}
		}

		// loaded surface no longer needed
		SDL_FreeSurface(loadedSurface);
	}

	// return sucess
	texture = tempTexture;
	return texture != nullptr;
}

// free texture if it exists
void Texture::free() const
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}
}


void Texture::render(const Rect<int> rect) const
{
	SDL_Rect renderQuad = { (rect.x1), (rect.y1),
							(rect.Width()), (rect.Height()) };

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}

void Texture::render(const Rect<int> rect, SDL_RendererFlip flip) const
{
	SDL_Rect renderQuad = { (rect.x1), (rect.y1),
							(rect.Width()), (rect.Height()) };

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, 0.0, NULL, flip);
}


void Texture::render(const RectF rect) const
{
	SDL_Rect renderQuad = { static_cast<int>(rect.x1),
							static_cast<int>(rect.y1),
							static_cast<int>(rect.Width()),
							static_cast<int>(rect.Height()) };

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}


// Renders texture with the roation specified
void Texture::render(const RectF rect, double rotation, VectorF aboutPoint) const
{
	// texture being displayed on the screen
	SDL_Rect renderQuad = { static_cast<int>(rect.x1),
						static_cast<int>(rect.y1),
						static_cast<int>(rect.Width()),
						static_cast<int>(rect.Height()) };

	// rotate about this point
	SDL_Point point = { (int)aboutPoint.x, (int)aboutPoint.y };

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, rotation, &point, SDL_FLIP_NONE);
}


// Renders part of the texture, e.g. a tile in a set
void Texture::renderSubTexture(const Rect<int> rect, const Rect<int> subRect) const
{
	// texture being displayed on the screen
	SDL_Rect renderQuad = { rect.x1, rect.y1, rect.Width(), rect.Height() };

	// the region of the texture being displayed on the screen
	SDL_Rect subQuad = { subRect.x1, subRect.y1, subRect.Width(), subRect.Height() };

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}


// Renders part of the texture, e.g. a tile in a set
void Texture::renderSubTexture(const RectF rect, const Rect<int> subRect) const
{
	// texture being displayed on the screen
	SDL_Rect renderQuad = { static_cast<int>(rect.x1),
						static_cast<int>(rect.y1),
						static_cast<int>(rect.Width()),
						static_cast<int>(rect.Height()) };

	// the region of the texture being displayed on the screen
	SDL_Rect subQuad = { subRect.x1, subRect.y1, subRect.Width(), subRect.Height() };

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}


// Renders part of the texture, e.g. a tile in a set with the roation specified
void Texture::renderSubTexture(const RectF rect, const Rect<int> subRect, double rotation, VectorF aboutPoint) const
{
	// texture being displayed on the screen
	SDL_Rect renderQuad = { static_cast<int>(rect.x1),
						static_cast<int>(rect.y1),
						static_cast<int>(rect.Width()),
						static_cast<int>(rect.Height()) };

	// the region of the texture being displayed on the screen
	SDL_Rect subQuad = { subRect.x1, subRect.y1, subRect.Width(), subRect.Height() };

	// rotate about this point
	SDL_Point point = { (int)aboutPoint.x, (int)aboutPoint.y };

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, rotation, &point, SDL_FLIP_NONE);
}


// Renders part of the texture, e.g. a tile in a set with the flip specified
void Texture::renderSubTexture(const RectF rect, const Rect<int> subRect, SDL_RendererFlip flip) const
{
	// texture being displayed on the screen
	SDL_Rect renderQuad = { static_cast<int>(rect.x1),
						static_cast<int>(rect.y1),
						static_cast<int>(rect.Width()),
						static_cast<int>(rect.Height()) };

	// the region of the texture being displayed on the screen
	SDL_Rect subQuad = { subRect.x1, subRect.y1, subRect.Width(), subRect.Height() };

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, 0.0, NULL, flip);
}


// Renders part of the texture, e.g. a tile in a set with the flip and alpha specified
void Texture::renderSubTexture(const RectF rect, const Rect<int> subRect, SDL_RendererFlip flip, const Uint8 tempAlpha)
{
	// texture being displayed on the screen
	SDL_Rect renderQuad = { static_cast<int>(rect.x1),
						static_cast<int>(rect.y1),
						static_cast<int>(rect.Width()),
						static_cast<int>(rect.Height()) };

	// the region of the texture being displayed on the screen
	SDL_Rect subQuad = { subRect.x1, subRect.y1, subRect.Width(), subRect.Height() };

	// Temporarily set the alpha 
	const Uint8 currentAlpha = alpha();
	setAlpha(tempAlpha);

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, 0.0, NULL, flip);

	// Set the alpha back to the default value
	setAlpha(currentAlpha);
}


// Renders part of the texture, e.g. a tile in a set with the roation specified
void Texture::renderSubTexture(const RectF rect, const Rect<int> subRect, double rotation, VectorF aboutPoint, SDL_RendererFlip flip) const
{
	// texture being displayed on the screen
	SDL_Rect renderQuad = { static_cast<int>(rect.x1),
						static_cast<int>(rect.y1),
						static_cast<int>(rect.Width()),
						static_cast<int>(rect.Height()) };

	// the region of the texture being displayed on the screen
	SDL_Rect subQuad = { subRect.x1, subRect.y1, subRect.Width(), subRect.Height() };

	// rotate about this point
	SDL_Point point = { (int)aboutPoint.x, (int)aboutPoint.y };

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, rotation, &point, flip);
}


void Texture::modifyAlpha(const int delta)
{
	Uint8 alpha;

	if (SDL_GetTextureAlphaMod(texture, &alpha) == 0)
	{
		alpha += delta;
		alpha = clamp(alpha, alphaMin, alphaMax);
		setAlpha(alpha);
	}
	else
		DebugPrint(Warning, "SDL_GetTextureAlphaMod function failed\n");
}


void Texture::setAlpha(const Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture, alpha);
}

const Uint8 Texture::alpha() const
{
	Uint8 texAlpha;

	SDL_GetTextureAlphaMod(texture, &texAlpha);

	return texAlpha;
}

