#include "pch.h"
#include "ScreenBuilder.h"

#include "Elements/UIElement.h"
#include "Elements/UIBox.h"
#include "Elements/UIButton.h"
#include "Elements/UISwitch.h"
#include "Elements/UITextBox.h"
#include "Elements/UISlider.h"

#include "Graphics/Texture.h"
#include "Graphics/TextureManager.h"


#include "Screens/ScreenHeaders.h"
#include "ScreenReader.h"

#include "Game/Camera/Camera.h"


void ScreenBuilder::populateScreen(Screen* screen) const
{
	ScreenType type = screen->type();

	BasicString fileName;
	type >> fileName;

	const ScreenReader reader;
	ScreenAttributes attributes = reader.readScreen(FileManager::Get()->findFile(FileManager::Config_Menus, fileName));
	ScreenLayers screenLayers = buildUIScreen(attributes);

#if DEBUG_CHECK
	if (screenLayers.size() == 0)
		DebugPrint(Warning, "No screen layers built for screen config %s\n", FileManager::Get()->findFile(FileManager::Config_Menus, fileName));
#endif

	screen->add(screenLayers);
}


ScreenLayers ScreenBuilder::buildUIScreen(const ScreenAttributes& screenAttributes) const
{
	ScreenLayers screenLayers = buildScreenLayers(screenAttributes);

	Elements parents = setParents(screenLayers, screenAttributes);
	setChildren(screenLayers, parents);
	formatElements(screenLayers);
	return screenLayers;
}




// --- Build screen layers --- //
ScreenLayers ScreenBuilder::buildScreenLayers(const ScreenAttributes& screenAttributes) const
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

		if (attributes.contains("fitting"))
		{
			if (attributes.at("fitting") == "clip")
			{
				box->setTextureClipping();
			}
		}

		element = box;
	}

	else if (type == "Button")
	{
		UIButton* button = new UIButton(attributes);
		
		TexturePacket textures = getButtonTextures(attributes.at("texture"));
		button->setTextures(textures.defaultTexture, textures.selected, textures.hovering);

		element = button;
	}

	else if (type == "Switch")
	{
		UISwitch* uiSwitch = new UISwitch(attributes);
		TexturePacket textures = getButtonTextures(attributes.at("texture"));

		uiSwitch->setTextures(textures.defaultTexture, textures.selected);
		element = uiSwitch;
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
void ScreenBuilder::setChildren(const ScreenLayers& screenLayers, Elements& parents) const
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

void ScreenBuilder::formatElements(ScreenLayers& screenLayers) const
{
	Elements elements = screenLayers.elementList();
	for (UIElement* element : elements)
	{
		autoSizeRect(element);
		repositionRelativeToParent(element);
		reformatText(element);
	}
}

void ScreenBuilder::autoSizeRect(UIElement* element) const
{
	RectF rect = element->rect();

	if (rect.Width() < 0)
	{
		UIBox* box = static_cast<UIBox*>(element);

		VectorF textureSize = box->texture()->originalDimentions;
		float ratioXY = textureSize.x / textureSize.y;

		float width = rect.Height() * ratioXY;

		VectorF position(rect.TopLeft());
		VectorF size(width, rect.Height());

		element->setRect(RectF(position, size));
	}
	else if (rect.Height() < 0)
	{
		UIBox* box = static_cast<UIBox*>(element);

		VectorF textureSize = box->texture()->originalDimentions;
		float ratioYX = textureSize.y / textureSize.x;

		float height = rect.Width() * ratioYX;

		VectorF position(rect.TopLeft());
		VectorF size(rect.Width(), height);

		element->setRect(RectF(position, size));
	}
}


void ScreenBuilder::reformatText(UIElement* element) const
{
	// set text sizes
	if (element->type() == UIElement::Type::TextBox)
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


Elements ScreenBuilder::setParents(ScreenLayers& layers, const ScreenAttributes& screenAttributes) const
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
					DebugPrint(Warning, "No parent was found with label: %s\n", attributes.at("parent").c_str());
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

#if DEBUG_CHECK
	if (!buttonNode)
	{
		DebugPrint(Warning, "Button config does not have a '%s' node\n", buttonType);
		return buttonTextures;
	}
#endif

	StringMap map = buttonNode.attributes();

	if (map.contains("texture"))
	{
		BasicString texture = map.at("texture");
		buttonTextures.defaultTexture = textures->getTexture(texture, FileManager::Image_UI);
	}

	if (map.contains("textureSelected"))
	{
		BasicString textureSelected = map.at("textureSelected");
		buttonTextures.selected = textures->getTexture(textureSelected, FileManager::Image_UI);
	}

	if (map.contains("textureHovering"))
	{
		BasicString textureHovering = map.at("textureHovering");
		buttonTextures.hovering = textures->getTexture(textureHovering, FileManager::Image_UI);
	}

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
