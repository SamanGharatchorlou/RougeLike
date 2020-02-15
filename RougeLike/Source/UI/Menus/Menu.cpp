#include "pch.h"
#include "Menu.h"

#include "Game/GameData.h"
#include "Graphics/TextureManager.h"

#include "Game/Cursor.h"
#include "Game/Camera.h"



void UILayer::render()
{
	for (unsigned int i = 0; i < mBoxes.size(); i++)
	{
		mBoxes[i].render();
	}

	for (unsigned int i = 0; i < mButtons.size(); i++)
	{
		mButtons[i].render();
	}
}


void UILayer::addBox(RectF rect, Texture* texture)
{
	mBoxes.push_back(UIBox(rect, texture));
}


void UILayer::addButton(RectF rect, Texture* defaultTexture, Texture* selectedTexture, UIButton::ACTION actionValue)
{
	mButtons.push_back(UIButton(rect, defaultTexture, selectedTexture, actionValue));
}


std::queue<UIButton::ACTION> UILayer::onPress(VectorF point)
{
	std::queue<UIButton::ACTION> actions;

	for (UIButton button : mButtons)
	{
		if (button.isPointInBounds(point))
		{
			button.setPressed(true);
			actions.push(button.getAction());
		}
	}

	return actions;
}


void Menu::init(MenuAttributes menuAttributes)
{
	for (LayerAttributes layerAttribute : menuAttributes)
	{
		addLayer(layerAttribute);
	}

	DebugPrint(Log, "Menu init'd with %d layers\n", mLayers.size());
}


void Menu::handleInput()
{
	ASSERT(Warning, mActions.empty(), "There ar %d unhandled button action(s)", mActions.size());

	if (mGameData->cursor->isPressed())
	{
		for (UILayer layer : mLayers)
		{
			if (layer.isActive())
			{
				// Get all actions for this layer
				std::queue<UIButton::ACTION> layerActions = layer.onPress(mGameData->cursor->getPosition());

				// Merge into Menu actions (i.e. all layers)
				while (!layerActions.empty())
				{
					mActions.push(layerActions.front());
					layerActions.pop();
				}
			}
		}
	}
}


void Menu::render()
{
	ASSERT(Warning, mLayers.size() > 0, "Menu must have at least 1 layer\n");

	for (int i = mLayers.size() - 1; i >= 0; i--)
	{
		mLayers[i].render();
	}
}


void Menu::addLayer(LayerAttributes layerAttributes)
{
	UILayer layer;
	int buttonCount = 0;

	for (Attributes attributes : layerAttributes)
	{
		float cameraWidth = mGameData->camera->getWidth();
		float cameraHeight = mGameData->camera->getHeight();

		// RectF
		float x = attributes.getFloat("x") * cameraWidth;
		float y = attributes.getFloat("y") * cameraHeight;
		float width = attributes.getFloat("width") * cameraWidth;
		float height = attributes.getFloat("height") * cameraHeight;
		RectF rect(VectorF(x, y), VectorF(width, height));

		// Texture
		std::string textureLabel = attributes.getString("texture");
		Texture* texture = mGameData->textureManager->getTexture(textureLabel);

		
		// Box
		if (strcmp(attributes.getString("type").c_str(), "box") == 0)
		{
			layer.addBox(rect, texture);
		}

		// Button
		else if (strcmp(attributes.getString("type").c_str(), "button") == 0)
		{
			Texture* textureSelected = nullptr;

			// Button may have 2 textures on/off
			if (attributes.contains("textureSelected"))
			{
				std::string textureLabel = attributes.getString("texture");
				textureSelected = mGameData->textureManager->getTexture(textureLabel);
			}

			std::string actionString = attributes.getString("action");
			UIButton::ACTION actionValue = getAction(actionString);

			layer.addButton(rect, texture, textureSelected, actionValue);
			buttonCount++;
		}
		else
		{
			DebugPrint(Warning, "The menu item %s not a box or button!\n", attributes.getString("type"));
		}
	}

	layer.setActive((bool)buttonCount);
	mLayers.push_back(layer);
}


UIButton::ACTION Menu::getAction(std::string action)
{
	if (action.empty())
		return UIButton::ACTION::NONE_0;
	else
	{
		// Get integer value of the last character
		int value = action.back() - '0' ;

		ASSERT(Warning, value > 0 && value < (int)UIButton::ACTION::COUNT,
			"Invalid buttion action %s generates value %d\n", action.c_str(), value);
		DebugPrint(Log, "Action %s generated value %d\n", action.c_str(), value);

		return (UIButton::ACTION)value;
	}
}