#include "pch.h"
#include "NetworkData.h"
#include "Input/InputManager.h"


InputPacket::InputPacket(const InputManager* input)
{
	std::queue<Button> inputButtons = input->getActiveButtons();
	for (int i = 0; i < 4; i++)
	{
		if (inputButtons.size() > 0)
		{
			buttons[i] = inputButtons.front();
			inputButtons.pop();
		}
		else
		{
			break;
		}
	}

	if (inputButtons.size() > 0)
	{
		DebugPrint(Log, "Input pack has more than 4 active buttons, consider increasing size of input pack button array to more than 4\n");
	}

	cursor = *input->getCursor();
}


void NetworkInputData::pack(const InputPacket& inputPacket)
{
	size = sizeof(inputPacket);
	memcpy(buffer, (void*)&inputPacket, size);
}


InputPacket NetworkInputData::unpack() const
{
	const InputPacket* inputData = reinterpret_cast<const InputPacket*>(buffer);
	return *inputData;
}