#pragma once

#include <iostream>
#include <Windows.h>
#include <chrono>
#include <ctime>


void Print(const char* string)
{
	OutputDebugStringA(string);
}

template<typename T>
void Print(T value)
{
	char buffer[100];
	itoa(value, buffer);
	OutputDebugStringA(buffer);
}

enum class BaseType
{
	None,
	AType,
	AnotherType
};




class UIElement
{
public:
	enum Type
	{
		None,
		Box,
		TextBox,
		Button,
		TextButton
	};

public:

	bool hasRect() { return (type == Box || type == TextBox || type == Button || type == TextButton); }

	// Attributes attributes;
	Type type;
	std::string id;

	// RectF rect;
};

class UIBox : public UIElement
{
public:
	void setRect(int rect);


private:
	// Texture* texture;
};

class UIButton : public UIBox
{
public:
	bool isPointInBounds(); // move this from UIBox to UIButton

private:
	// all the button things
};



class EditUIRectEvent
{
	std::string id;
	int rect;
};


// UI editor - make this a global?
class UIManager
{
	void handleEvent(int event, int data);

};

void UIManager::handleEvent(int event, int data)
{
	if (event == EditUIRect)
	{
		EditUIRectEvent& data = static_cast<EditUIRectEvent&>(data);

		UIElement* element = find(data.id);

		if (element->hasRect())
		{
			UIBox* box = static_cast<UIBox*>(element);
			box->setRect(data.rect);
		}

	}
}



class Derived : public Base
{
public:
	Derived(int value) : Base(BaseType::AType), aValue(value) { }

	void print() { printf("derived"); }
	int aValue;
};


void PrintMyValue(Base& baseClassObj)
{
	baseClassObj.print();
}


int main()
{
	Derived myClass(50);

	PrintMyValue(myClass);

	return 0;
}



