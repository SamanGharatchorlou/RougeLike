#include "pch.h"
#include "Texture.h"

Texture::Texture() : texture(nullptr), renderer(nullptr) { }


Texture::~Texture()
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}
}


bool Texture::loadFromFile(const BasicString& filePath)
{
	// remove any existing texture
	if (texture)
	{
		SDL_DestroyTexture(texture);
	}

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




void Texture::render(const RectF& rect, SDL_RendererFlip flip) const
{
	SDL_Rect renderQuad = toSDLRect(rect);

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, 0.0, NULL, flip);
}


void Texture::render(const RectF& rect) const
{
	SDL_Rect renderQuad = toSDLRect(rect);

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}


// Renders texture with the roation specified
// NOTE: the about point is relative to the rect e.g. about the center would be rect.size()/2, not rect.center()
void Texture::render(const RectF& rect, double rotation, VectorF aboutPoint) const
{
	SDL_Rect renderQuad = toSDLRect(rect);
	SDL_Point point = { (int)(aboutPoint.x + 0.5f), (int)(aboutPoint.y + 0.5f) };

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, rotation, &point, SDL_FLIP_NONE);
}


// Renders part of the texture, e.g. a tile in a set with the roation specified
void Texture::render(const RectF& rect, double rotation, VectorF aboutPoint, SDL_RendererFlip flip) const
{
	SDL_Rect renderQuad = toSDLRect(rect);
	SDL_Point point = { (int)aboutPoint.x, (int)aboutPoint.y };

	SDL_RenderCopyEx(renderer, texture, nullptr, &renderQuad, rotation, &point, flip);
}


// Renders part of the texture, e.g. a tile in a set
void Texture::renderSubTexture(const RectF& rect, const RectF& subRect) const
{
	SDL_Rect renderQuad = toSDLRect(rect);
	SDL_Rect subQuad = toSDLRect(subRect);

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}


// Renders part of the texture, e.g. a tile in a set with the rotation specified
// NOTE: the about point is relative to the rect e.g. about the center would be rect.size()/2, not rect.center()
void Texture::renderSubTexture(const RectF& rect, const RectF& subRect, double rotation, VectorF aboutPoint) const
{
	SDL_Rect renderQuad = toSDLRect(rect);
	SDL_Rect subQuad = toSDLRect(subRect);

	// rotate about this point
	SDL_Point point = { (int)aboutPoint.x, (int)aboutPoint.y };

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, rotation, &point, SDL_FLIP_NONE);
}


// Renders part of the texture, e.g. a tile in a set with the flip specified
void Texture::renderSubTexture(const RectF& rect, const RectF& subRect, SDL_RendererFlip flip) const
{
	SDL_Rect renderQuad = toSDLRect(rect);
	SDL_Rect subQuad = toSDLRect(subRect);

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, 0.0, NULL, flip);
}


// Renders part of the texture, e.g. a tile in a set with the flip and alpha specified
void Texture::renderSubTexture(const RectF& rect, const RectF& subRect, SDL_RendererFlip flip, Uint8 tempAlpha)
{
	SDL_Rect renderQuad = toSDLRect(rect);
	SDL_Rect subQuad = toSDLRect(subRect);

	// Temporarily set the alpha 
	const Uint8 currentAlpha = alpha();

	SDL_SetTextureAlphaMod(texture, tempAlpha);


	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, 0.0, NULL, flip);

	// Set the alpha back to the default value
	SDL_SetTextureAlphaMod(texture, tempAlpha);
}


// Renders part of the texture, e.g. a tile in a set with the flip and alpha specified. Also apply a colour mod.
void Texture::renderSubTexture(const RectF& rect, const RectF& subRect, SDL_RendererFlip flip, RenderColour colourMod)
{
	SDL_Rect renderQuad = toSDLRect(rect);
	SDL_Rect subQuad = toSDLRect(subRect);

	// Apply temporary colour modulation 
	SDL_SetTextureColorMod(texture, colourMod.r, colourMod.g, colourMod.b);

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, 0.0, NULL, flip);

	// Remove colour modulation
	SDL_SetTextureColorMod(texture, 255, 255, 255);
}


// Renders part of the texture, e.g. a tile in a set with the roation specified
void Texture::renderSubTexture(const RectF& rect, const RectF& subRect, double rotation, VectorF aboutPoint, SDL_RendererFlip flip) const
{
	SDL_Rect renderQuad = toSDLRect(rect);
	SDL_Rect subQuad = toSDLRect(subRect);

	// rotate about this point
	SDL_Point point = { (int)aboutPoint.x, (int)aboutPoint.y };

	SDL_RenderCopyEx(renderer, texture, &subQuad, &renderQuad, rotation, &point, flip);
}


const Uint8 Texture::alpha() const
{
	Uint8 texAlpha;

	SDL_GetTextureAlphaMod(texture, &texAlpha);

	return texAlpha;
}

