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

		for (Attributes attributes : layerAttribute)
		{
			const char* type = attributes.getString("type").c_str();

			// Box
			if (strcmp(type, "Box") == 0)
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
			else if (strcmp(type, "BasicText") == 0)
			{
				UIBasicText::Data data;
				fillBasicTextData(data, attributes);

				layer->addElement(new UIBasicText(data));
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
	data.rect = generateRect(attributes);
	data.id = attributes.getString("id");
}


void ScreenDecoder::fillBoxData(UIBox::Data& data, Attributes& attributes)
{
	fillElementData(data, attributes);

	// Texture
	std::string textureLabel = attributes.getString("texture");
	data.texture = mGameData->textureManager->getTexture(textureLabel);
}


void ScreenDecoder::fillBasicTextData(UIBasicText::Data& data, Attributes& attributes)
{
	fillElementData(data, attributes);

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


void ScreenDecoder::fillButtonData(UIButton::Data& data, Attributes& attributes)
{
	// Box component
	fillBoxData(data, attributes);

	data.highlightedTexture = nullptr;

	// Button may have 2 textures on/off
	if (attributes.contains("textureSelected"))
	{
		std::string textureLabel = attributes.getString("texture");
		data.highlightedTexture = mGameData->textureManager->getTexture(textureLabel);
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


RectF ScreenDecoder::generateRect(Attributes& attributes) const
{
	float cameraWidth = mGameData->camera->getWidth();
	float cameraHeight = mGameData->camera->getHeight();

	// RectF
	float x = 0.0f;
	if (attributes.contains("x_p"))
	{
		x = attributes.getFloat("x_p") * cameraWidth;
	}
	else if (attributes.contains("x"))
	{
		x = attributes.getFloat("x");
	}
	else
	{
		DebugPrint(Warning, "No x value was found, setting x = 0\n");
	}

	float y = 0.0f;
	if (attributes.contains("y_p"))
	{
		y = attributes.getFloat("y_p") * cameraHeight;
	}
	else if (attributes.contains("y"))
	{
		y = attributes.getFloat("y");
	}
	else
	{
		DebugPrint(Warning, "No y value was found, setting y = 0\n");
	}

	float width = 0.0f;
	if (attributes.contains("width_p"))
	{
		width = attributes.getFloat("width_p") * cameraWidth;
	}
	else if (attributes.contains("width"))
	{
		width = attributes.getFloat("width");
	}
	else
	{
		DebugPrint(Warning, "No width value was found, setting width = 0\n");
	}

	float height = 0.0f;
	if (attributes.contains("height_p"))
	{
		height = attributes.getFloat("height_p") * cameraHeight;
	}
	else if (attributes.contains("height"))
	{
		height = attributes.getFloat("height");
	}
	else
	{
		DebugPrint(Warning, "No height value was found, setting height = 0\n");
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