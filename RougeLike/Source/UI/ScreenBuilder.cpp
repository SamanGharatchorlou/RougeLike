#include "pch.h"
#include "ScreenBuilder.h"

#include "Elements/UIBox.h"
#include "Elements/UIButton.h"
#include "Elements/UIElement.h"
#include "Elements/UITextBox.h"
#include "Elements/UISlider.h"

#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"


#include "Screens/ScreenHeaders.h"
#include "ScreenReader.h"


Screen* ScreenBuilder::buildNewScreen(const BasicString& config)
{
	const ScreenReader reader;
	ScreenAttributes attributes = reader.readScreen(config);
	ScreenLayers screenLayers = buildUIScreen(attributes);

	Screen* screen = createNewScreen(config);
	screen->add(screenLayers);
	
	return screen;
}


Screen* ScreenBuilder::createNewScreen(const BasicString& config)
{
	Screen* screen = nullptr;
	BasicString screenName = FileManager::Get()->getItemName(config);

	if (screenName == "GameScreen")
		screen = new GameScreen;

	else if (screenName == "PauseScreen")
		screen = new PauseScreen;

	else if (screenName == "CharacterSelectionScreen")
		screen = new CharacterSelectionScreen;

	else if (screenName == "SettingsScreen")
		screen = new SettingsScreen;

	return screen;
}


ScreenLayers ScreenBuilder::buildUIScreen(const ScreenAttributes& screenAttributes)
{
	ScreenLayers screenLayers = buildScreenLayers(screenAttributes);

	Elements parents = setParents(screenLayers, screenAttributes);
	setChildren(screenLayers, parents);
	formatElements(screenLayers);
	return screenLayers;
}




// --- Build screen layers --- //
ScreenLayers ScreenBuilder::buildScreenLayers(const ScreenAttributes& screenAttributes)
{
	ScreenLayers screenLayers;

	for (const LayerAttributes& layerAttributes : screenAttributes)
	{
		ScreenLayer layer = buildLayer(layerAttributes);
		screenLayers.add(layer);
	}

	return screenLayers;
}


ScreenLayer ScreenBuilder::buildLayer(const LayerAttributes& layerAttributes) const
{
	ScreenLayer layer("");
	
	for (const StringMap& attributes : layerAttributes)
	{
		UIElement* element = buildElement(attributes);
		layer.elements.push_back(element);
	}

	return layer;
}


UIElement* ScreenBuilder::buildElement(const StringMap& attributes) const
{
	UIElement* element = nullptr;
	BasicString type = attributes.at("type");

	if (type == "Element")
	{
		element = new UIElement(attributes);
	}

	else if (type == "Box")
	{
		UIBox* box = new UIBox(attributes);
		Texture* texture = getTexture(attributes);
		box->setTexture(texture);

		element = box;
	}

	else if (type == "Button")
	{
		UIButton* button = new UIButton(attributes);
		
		TexturePacket textures = getButtonTextures(attributes.at("texture"));
		button->setTextures(textures.defaultTexture, textures.selected, textures.hovering);

		element = button;
	}

	else if (type == "Slider")
	{
		UISlider* slider = new UISlider(attributes);

		XMLParser parser(FileManager::Get()->findFile(FileManager::Configs, "Sliders"));
		XMLNode node = parser.rootChild(attributes.at("slider"));
		StringMap sliderAttributes = node.attributes();

		buildSlider(slider, sliderAttributes);

		element = slider;
	}

	else if (type == "TextBox")
	{
		element = new UITextBox(attributes);
	}

	return element;
}


void ScreenBuilder::buildSlider(UISlider* slider, const StringMap& attributes) const
{
	const TextureManager* textures = TextureManager::Get();

	// Container
	Texture* container = textures->getTexture(attributes.at("Container"), FileManager::Image_UI);

	// Bar
	UIBox::Data barData;
	barData.parent = slider;
	barData.texture = textures->getTexture(attributes.at("Bar"), FileManager::Image_UI);
	UIBox* bar = new UIBox(barData);

	// Button
	UIButton::Data handleData;
	handleData.parent = slider;
	handleData.defaultTexture = textures->getTexture(attributes.at("Handle"), FileManager::Image_UI);
	UIButton* sliderHandle = new UIButton(handleData);

	slider->setComponents(container, sliderHandle, bar);
}


// --- Fill element data --- //
void ScreenBuilder::setChildren(const ScreenLayers& screenLayers, Elements& parents)
{
	Elements elements = screenLayers.elementList();

	for (UIElement* parent : parents)
	{
		for (UIElement* element : elements)
		{
			if (parent == element->parent())
			{
				parent->addChild(element);
			}
		}

		ASSERT(Warning, parent->children().size() != 0, "Parent %s could find no children\n", parent->id());
	}
}

void ScreenBuilder::formatElements(ScreenLayers& screenLayers)
{
	Elements elements = screenLayers.elementList();
	for (UIElement* element : elements)
	{
		repositionRelativeToParent(element);
		reformatText(element);
	}
}


void ScreenBuilder::reformatText(UIElement* element)
{
	// set text sizes
	if (element->hasText())
	{
		UITextBox* textBox = static_cast<UITextBox*>(element);

		// If no font size has been given auto size the font to fit the box
		if (!textBox->fontSize())
			textBox->autoSizeFont();

		textBox->align();
	}
}


void ScreenBuilder::repositionRelativeToParent(UIElement* element) const
{
	RectF rect = element->rect();

	if (element->parent() != nullptr)
	{
		RectF parentRect = element->parent()->rect();
		VectorF parentPosition = parentRect.TopLeft();
		rect = rect.Translate(parentPosition);
	}

	element->setRect(rect);
}


Elements ScreenBuilder::setParents(ScreenLayers& layers, const ScreenAttributes& screenAttributes)
{
	Elements parents;

	for (const LayerAttributes& layerAttributes : screenAttributes)
	{
		for (const StringMap& attributes : layerAttributes)
		{
			if (attributes.contains("parent"))
			{
				BasicString parentId = attributes.at("parent");
				UIElement* parent = layers.find(parentId);

				if (parent != nullptr)
				{
					BasicString elementId = attributes.at("id");
					UIElement* element = layers.find(elementId);

					element->setParernt(parent);
					parents.push_back(parent);
				}
				else
				{
					DebugPrint(Log, "No parent was found with label: %s\n", attributes.at("parent").c_str());
				}
			}
		}
	}

	return parents;
}



// --- Get Textures --- //
TexturePacket ScreenBuilder::getButtonTextures(const BasicString& buttonType) const
{
	TexturePacket buttonTextures;
	const TextureManager* textures = TextureManager::Get();

	XMLParser button(FileManager::Get()->findFile(FileManager::Configs, "Buttons"));
	XMLNode buttonNode = button.rootChild(buttonType);

#if _DEBUG
	if (!buttonNode)
	{
		DebugPrint(Warning, "Button config does not have a '%s' node\n", buttonType);
	}
#endif

	StringMap map = buttonNode.attributes();

	BasicString texture = map.at("texture");
	buttonTextures.defaultTexture = textures->getTexture(texture, FileManager::Image_UI);

	BasicString textureSelected = map.at("textureSelected");
	buttonTextures.selected = textures->getTexture(textureSelected, FileManager::Image_UI);

	BasicString textureHovering = map.at("textureHovering");
	buttonTextures.hovering = textures->getTexture(textureHovering, FileManager::Image_UI);

	return buttonTextures;
}


Texture* ScreenBuilder::getTexture(const StringMap& attributes) const
{
	const TextureManager* textures = TextureManager::Get();
	Texture* texture = nullptr;

	if (attributes.contains("texture"))
	{
		BasicString textureLabel = attributes.at("texture");
		texture = textures->getTexture(textureLabel, FileManager::Image_UI);
	}

	return texture;
}
