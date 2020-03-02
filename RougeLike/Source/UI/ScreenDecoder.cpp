#include "pch.h"
#include "ScreenDecoder.h"

#include "Game/GameData.h"
#include "Graphics/TextureManager.h"
#include "Game/Camera.h"

#include "UI/Screens/UILayer.h"


typedef std::vector<Attributes> LayerAttributes;
typedef std::vector<LayerAttributes> ScreenAttributes;


ScreenAttributes ScreenDecoder::getScreenAttributes(std::string config)
{
	xmlParser.parseXML(config);

	xmlNode screenNode = xmlParser.getRootNode();
	ASSERT(Error, strcmp(screenNode->name(), "screen") == 0, "Incorrect attributes node\n");

	xmlNode layerNode = screenNode->first_node("layer");
	ASSERT(Error, strcmp(layerNode->name(), "layer") == 0, "Incorrect attributes node\n");

	ScreenAttributes screenAttributes;

	// Get all layers
	while (layerNode != nullptr)
	{
		std::vector<Attributes> layerAttibutes;

		xmlNode childNode = layerNode->first_node();

		// UI component
		while (childNode != nullptr)
		{
			Attributes itemAttributes = xmlParser.getAttributes(childNode);
			itemAttributes.add("type", childNode->name());

			// Text component
			xmlNode node = childNode->first_node();
			if (childNode->first_node())
			{
				Attributes textAttributes = xmlParser.getAttributes(childNode->first_node());

				// text to be displayed
				std::string text = childNode->first_node()->value();
				textAttributes.add("text", text);

				itemAttributes.merge(textAttributes);
			}

			layerAttibutes.push_back(itemAttributes);

			childNode = childNode->next_sibling();
		}

		screenAttributes.push_back(layerAttibutes);

		layerNode = layerNode->next_sibling();
	}

	return screenAttributes;
}


std::vector<UILayer*> ScreenDecoder::buildUIScreenLayers(ScreenAttributes& attributes)
{
	std::vector<UILayer*> layers;	

	for (LayerAttributes layerAttribute : attributes)
	{
		UILayer* layer = new UILayer;
		currentLayer = layer;

		for (Attributes attributes : layerAttribute)
		{
			const char* type = attributes.getString("type").c_str();

			// Element
			if (strcmp(type, "Element") == 0)
			{
				UIElement::Data data;
				fillElementData(data, attributes);

				layer->addElement(new UIElement(data));
			}

			// Box
			else if (strcmp(type, "Box") == 0)
			{
				UIBox::Data data;
				fillBoxData(data, attributes);

				layer->addElement(new UIBox(data));
			}

			// Button
			else if (strcmp(type, "Button") == 0)
			{
				UIButton::Data data;
				fillButtonData(data, attributes);

				layer->addElement(new UIButton(data));
			}

			// Text Button
			else if (strcmp(type, "TextButton") == 0)
			{
				UITextButton::Data data;
				fillTextButtonData(data, attributes);

				layer->addElement(new UITextButton(data));
			}

			// Text Box
			else if (strcmp(type, "TextBox") == 0)
			{
				UITextBox::Data data;
				fillTextBoxtData(data, attributes);

				UITextBox* textBox = new UITextBox(data);

				if (textBox->fontSize() == 0)
				{
					textBox->autoSizeFont();
				}

				layer->addElement(textBox);
			}

			else
			{
				DebugPrint(Warning, "The Screen item %s not a box or button!\n", attributes.getString("type").c_str());
			}
		}

		layers.push_back(layer);
	}

	ASSERT(Warning, layers.size() > 0, "This screen has no layers\n");
	return layers;
}



// --- Populate UI Element data packets --- //

void ScreenDecoder::fillElementData(UIElement::Data& data, Attributes& attributes)
{
	// Set parent first so the rect can be correctly positioned relative to it
	data.parent = nullptr;
	if (attributes.contains("parent"))
	{
		for (unsigned int i = 0; i < currentLayer->elements().size(); i++)
		{
			const UIElement* element = currentLayer->element(i);

			if (strcmp(element->id().c_str(), attributes.getString("parent").c_str()) == 0)
			{
				data.parent = element;
			}
		}

		ASSERT(Warning, data.parent, "No parent was found with lable: %s\n", attributes.getString("parent").c_str());
	}

	data.rect = generateRect(attributes, data.parent);
	data.id = attributes.getString("id");
}


void ScreenDecoder::fillBoxData(UIBox::Data& data, Attributes& attributes)
{
	fillElementData(data, attributes);

	// Texture
	if (attributes.contains("texture"))
	{
		std::string textureLabel = attributes.getString("texture");
		data.texture = mGameData->textureManager->getTexture(textureLabel);
	}
	else
		data.texture = nullptr;
}


void ScreenDecoder::fillTextBoxtData(UITextBox::Data& data, Attributes& attributes)
{
	fillBoxData(data, attributes);

	// Text
	data.text = attributes.getString("text");
	data.font = attributes.getString("font");

	data.colour = {
		(Uint8)attributes.getInt("r"),
		(Uint8)attributes.getInt("g"),
		(Uint8)attributes.getInt("b")
	};

	if (attributes.contains("ptSize"))
	{
		data.ptSize = attributes.getInt("ptSize");
	}
	else
	{
		data.ptSize = 0;
	}
}


void ScreenDecoder::fillButtonData(UIButton::Data& data, Attributes& attributes)
{
	// Box component
	fillBoxData(data, attributes);

	// Button may have 2 textures on/off
	if (attributes.contains("textureSelected"))
	{
		std::string textureLabel = attributes.getString("textureSelected");
		data.highlightedTexture = mGameData->textureManager->getTexture(textureLabel);
	}
	else
	{
		data.highlightedTexture = data.texture;
	}

	// Action
	std::string actionString = attributes.getString("action");
	data.action = getAction(actionString);

	// Id
	data.id = attributes.getString("id");
}


void ScreenDecoder::fillTextButtonData(UITextButton::Data& data, Attributes& attributes)
{
	// Button component
	fillButtonData(data, attributes);
	
	// Text
	data.text = attributes.getString("text");
	data.font = attributes.getString("font");
	data.ptSize = attributes.getInt("ptSize");
	data.colour = {
		(Uint8)attributes.getInt("r"),
		(Uint8)attributes.getInt("g"),
		(Uint8)attributes.getInt("b")
	};
}


RectF ScreenDecoder::generateRect(Attributes& attributes, const UIElement* parent) const
{
	// RectF
	float x = attributes.getFloat("x");
	float y = attributes.getFloat("y");

	float width = -1;
	float height = -1;

	if (attributes.contains("width"))
	{
		width = attributes.getFloat("width");
	}

	if (attributes.contains("height"))
	{
		height = attributes.getFloat("height");
	}


	// Adjust relative to the parent rect
	// No need to convert to pixles as parent already has been
	if (parent != nullptr)
	{
		RectF parentRect = parent->getRect();

		x = parentRect.x1 + x * parentRect.Width();
		y = parentRect.y1 + y * parentRect.Height();
		width = width * parentRect.Width();
		height = height * parentRect.Height();
	}
	// Convert relative positions to pixles
	else
	{
		VectorF screenSize = mGameData->camera->getSize();

		x = x * screenSize.x;
		y = y * screenSize.y;
		width = width * screenSize.x;
		height = height * screenSize.y;
	}

	return RectF(VectorF(x, y), VectorF(width, height));
}



UIButton::Action ScreenDecoder::getAction(std::string action)
{
	if (action.empty())
		return UIButton::None_0;
	else
	{
		// Get integer value of the last character
		int value = action.back() - '0';

		ASSERT(Warning, value > 0 && value < (int)UIButton::Action::COUNT,
			"Invalid buttion action %s generates value %d\n", action.c_str(), value);
		DebugPrint(Log, "Action %s generated value %d\n", action.c_str(), value);

		return (UIButton::Action)value;
	}
}