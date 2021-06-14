#pragma once

#include "Input/Button.h"
#include "Input/Cursor.h"

class InputManager;


struct InputPacket
{
	InputPacket() { }
	InputPacket(const InputManager* input);

	//bool hasData = false;
	Button buttons[4];
	Cursor cursor;
};


struct NetworkInputData
{
	NetworkInputData() : size(0) { }

	void pack(const InputPacket& inputPacket);
	InputPacket unpack() const;

	int size;
	char buffer[256] = { 0 };
};
