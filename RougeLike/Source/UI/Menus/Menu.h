#pragma once

#include "UI/UIButton.h"

class Texture;
struct GameData;

typedef std::vector<Attributes> LayerAttributes;
typedef std::vector<LayerAttributes> MenuAttributes;


class UILayer
{
public:
	void render();

	std::queue<UIButton::ACTION> onPress(VectorF point);

	inline void setActive(bool active) { mIsActive = active; }
	inline bool isActive() const { return mIsActive; }

	void addBox(RectF rect, Texture* texture);
	void addButton(RectF rect, Texture* defaultTexture, Texture* selectedTexture, UIButton::ACTION actionValue);

	const std::vector<UIButton>& getButtons() const { return mButtons; }

private:
	std::vector<UIBox> mBoxes;
	std::vector<UIButton> mButtons;

	bool mIsActive;
};


class Menu
{
public:
	Menu(GameData* gameData) : mGameData(gameData) { }

	void init(MenuAttributes menuAtributes);
	void handleInput();
	void render();

	virtual void enter() = 0;
	virtual void update() = 0;
	virtual void exit() = 0;

private:
	void addLayer(LayerAttributes layerAttributes);
	UIButton::ACTION getAction(std::string action);

protected:
	GameData* mGameData;

	std::vector<UILayer> mLayers;

	std::queue<UIButton::ACTION> mActions;
};