#pragma once

#include "Input/Button.h"
#include "Input/Cursor.h"

struct NetworkData
{
	bool hasData = false;
	int size = 0;
	char buffer[256] = { 0 };
};


struct InputPacket
{
	bool hasData = false;
	Button button;
	Cursor cursor;
	//std::vector<Button> mInputs;
};