#pragma once

#include "ScreenTypes.h"

class UIButton;
class UIElement;

class InputManager;
class TextureManager;


class Screen
{
public:
	enum Type
	{
		None,
		CharacterSelection,
		Game,
		Pause,
		Settings
	};


public:
	Screen() : mTextures(nullptr) { }
	Screen(const TextureManager* textures) : mTextures(textures) { };
	virtual ~Screen();

	void add(ScreenLayers& layers);

	ScreenLayers& layers() { return mScreenLayers; }

	void updateButtons(const InputManager* input);

	virtual void enter() = 0;
	virtual void handleInput(const InputManager* input) = 0;
	virtual void update(float dt) = 0;
	virtual void exit() = 0;
	virtual void render();

	virtual Type type() = 0;

	UIElement* find(const BasicString& id);
	UIButton* findButton(const BasicString& id);


protected:
	const TextureManager* mTextures;
	ScreenLayers mScreenLayers;
};



class NullScreen : public Screen
{
public:
	void enter() { }
	void handleInput(const InputManager* input) { }
	void update(float dt) { }
	void exit() { }
	void render() { }
	Type type() { return Type::None; }
};