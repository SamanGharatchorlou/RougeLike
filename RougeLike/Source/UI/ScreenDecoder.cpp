#include "pch.h"
#include "ScreenDecoder.h"

#include "Game/Camera.h"
#include "Graphics/TextureManager.h"

#include "UI/Screens/UILayer.h"


ScreenAttributes ScreenDecoder::getScreenAttributes(const std::string& config)
{
	xmlParser.parseXML(config);

	xmlNode screenNode = xmlParser.rootNode();
	ASSERT(Error, strcmp(screenNode->name(), "screen") == 0, "Incorrect attributes node\n");

	xmlNode layerNode = screenNode->first_node("layer");
	ASSERT(Error, strcmp(layerNode->name(), "layer") == 0, "Incorrect attributes node\n");

	ScreenAttributes screenAttributes;

	// Get all layers
	while (layerNode != nullptr)
	{
		LayerAttributes layerAttibutes;

		xmlNode childNode = layerNode->first_node();

		// UI component
		while (childNode != nullptr)
		{
			Attributes itemAttributes = xmlParser.attributes(childNode);
			itemAttributes.add("type", childNode->name());

			// Text component
			xmlNode node = childNode->first_node();
			if (childNode->first_node())
			{
				Attributes textAttributes = xmlParser.attributes(childNode->first_node());

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


Layers ScreenDecoder::buildUIScreenLayers(ScreenAttributes& screenAttributes)
{
	Layers layers;

	for (LayerAttributes layerAttribute : screenAttributes)
	{
		UILayer* layer = new UILayer;
		currentLayer = layer; // TODO: expand this to all layers. also delay this so the rects are set after its all been parsed

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

			// Text Box
			else if (strcmp(type, "TextBox") == 0)
			{
				UITextBox::Data data;
				fillTextBoxData(data, attributes);

				layer->addElement(new UITextBox(data));
			}

			else
			{
				DebugPrint(Warning, "The Screen item %s not a box or button!\n", attributes.getString("type").c_str());
			}
		}

		layers.push_back(layer);
	}

	// Set UI element parents and cache parents for next step
	Elements parents = setParents(layers, screenAttributes);

	//  List all parent children
	setChildren(parents, layers);

	// Set element rects relative to parents
	setRects(layers);

	ASSERT(Warning, layers.size() > 0, "This screen has no layers\n");
	return layers;
}

void ScreenDecoder::setRects(Layers layers)
{
	for (UILayer* layer : layers)
	{
		for (UIElement* element : layer->elements())
		{
			setRect(element);

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
	}
}


void ScreenDecoder::setChildren(Elements parents, Layers layers)
{
	// Third Pass: setup childred
	for (UIElement* parent : parents)
	{
		for (UILayer* layer : layers)
		{
			for (UIElement* element : layer->elements())
			{
				if (element->parent() == parent)
				{
					parent->addChild(element);
				}
			}
		}

		ASSERT(Warning, parent->children().size() != 0, "Parernt %s could find no children\n", parent->id());
	}
}


Elements ScreenDecoder::setParents(Layers layers, ScreenAttributes& screenAttributes)
{
	// Store parents for more effcient third pass
	Elements parentElements;

	for (unsigned int layerIndex = 0; layerIndex < screenAttributes.size(); layerIndex++)
	{
		// TODO: does this setup a reference or value?
		LayerAttributes layerAttributes = screenAttributes[layerIndex];

		for (unsigned int elementIndex = 0; elementIndex < layerAttributes.size(); elementIndex++)
		{
			Attributes elementAttributes = layerAttributes[elementIndex];

			if (elementAttributes.contains("parent"))
			{
				UIElement* element = layers[layerIndex]->element(elementIndex);

				// search for parent with this id
				UIElement* parent = findElement(layers, elementAttributes.getString("parent"));
				ASSERT(Warning, parent != nullptr, "No parent was found with label: %s\n", elementAttributes.getString("parent").c_str());

				element->setParernt(parent);

				parentElements.push_back(parent);
			}
		}
	}

	// Remove duplicates
	parentElements.erase(unique(parentElements.begin(), parentElements.end()), parentElements.end());
	return parentElements;
}


// --- Populate UI Element data packets --- //

void ScreenDecoder::fillElementData(UIElement::Data& data, Attributes& attributes) const
{
	// Allow these attributes not to be set, assuming they have a parent rect
	float x = attributes.contains("x") ? attributes.getFloat("x") : 12345.0f;
	float y = attributes.contains("y") ? attributes.getFloat("y") : 12345.0f;

	float width = attributes.contains("width") ? attributes.getFloat("width") : 0.0f;
	float height = attributes.contains("height") ? attributes.getFloat("height") : 0.0f;

#if _DEBUG
	if (x == 12345.0f || y == 12345.0f || width == 0.0f || height == 0.0f)
	{
		if (!attributes.contains("parent"))
		{
			DebugPrint(Warning, "No x, y, width or height parameter has been set and \
								 there is no parent to default the value to. This is invalid\n");
		}
	}
#endif

	data.rect = RectF(VectorF(x, y), VectorF(width, height));

	// Not every element needs an id
	if(attributes.contains("id"))
		data.id = attributes.getString("id");
}


void ScreenDecoder::fillBoxData(UIBox::Data& data, Attributes& attributes) const
{
	fillElementData(data, attributes);

	// Texture
	if (attributes.contains("texture"))
	{
		std::string textureLabel = attributes.getString("texture");
		data.texture = tm->getTexture(textureLabel);
	}
	else
		data.texture = nullptr;

#if _DEBUG
	if (attributes.contains("debugDraw"))
		data.drawRect = true;
	else
		data.drawRect = false;
#endif
}


void ScreenDecoder::fillTextBoxData(UITextBox::Data& data, Attributes& attributes) const
{
	fillBoxData(data, attributes);

	// Text
	data.text = attributes.getString("text");
	data.font = attributes.contains("font") ? attributes.getString("font") : "default.ttf";

	data.colour = {
		(Uint8)attributes.getInt("r"),
		(Uint8)attributes.getInt("g"),
		(Uint8)attributes.getInt("b")
	};

	data.ptSize = attributes.contains("ptSize") ? attributes.getInt("ptSize") : 0;
	data.aligment = attributes.contains("align") ? attributes.getString("align") : "";
}


void ScreenDecoder::fillButtonData(UIButton::Data& data, Attributes& attributes) const
{
	// Box component
	fillBoxData(data, attributes);

	// Button may have 2 textures on/off
	if (attributes.contains("textureSelected"))
	{
		std::string textureLabel = attributes.getString("textureSelected");
		data.highlightedTexture = tm->getTexture(textureLabel);
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


void ScreenDecoder::setRect(UIElement* element)
{
	RectF relativeRect = element->rect();
	RectF absoluteRect;

	// Adjust relative to the parent rect
	// No need to convert to pixles as parent already has been
	if (element->parent() != nullptr)
	{
		RectF parentRect = element->parent()->rect();

		float x = (relativeRect.x1 == 12345.0f) ? 0.0f : relativeRect.x1;
		float y = (relativeRect.y1 == 12345.0f) ? 0.0f : relativeRect.y1;

		absoluteRect.x1 = parentRect.x1 + x * parentRect.Width();
		absoluteRect.y1 = parentRect.y1 + y * parentRect.Height();

		float width = relativeRect.Width() == 0.0f ? 1.0f : relativeRect.Width();
		float height = relativeRect.Height() == 0.0f ? 1.0f : relativeRect.Height();

		absoluteRect.SetWidth(width * parentRect.Width());
		absoluteRect.SetHeight(height * parentRect.Height());
	}
	// Convert relative positions to pixles
	else
	{
		VectorF screenSize = Camera::Get()->size();

		absoluteRect.x1 = relativeRect.x1 * screenSize.x;
		absoluteRect.y1 = relativeRect.y1 * screenSize.y;
		absoluteRect.SetWidth(relativeRect.Width() * screenSize.x);
		absoluteRect.SetHeight(relativeRect.Height() * screenSize.y);
	}

	element->setRect(absoluteRect);
}



UIButton::Action ScreenDecoder::getAction(const std::string& action) const
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



UIElement* ScreenDecoder::findElement(Layers layers, const std::string& id)
{
	for (const UILayer* layer : layers)
	{
		for (UIElement* element : layer->elements())
		{
			if (element->id() == id)
					return element;
		}
	}

	DebugPrint(Warning, "No element with the id %s was found\n", id.c_str());
	return nullptr;
}