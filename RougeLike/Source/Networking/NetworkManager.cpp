#include "pch.h"
#include "NetworkManager.h"
#include "Networking/Client.h"
#include "Networking/Server.h"

#include "Input/InputManager.h"
#include "Actors/Player/PlayerManager.h"
#include "Debug/FrameRateController.h"


NetworkManager::NetworkManager()
{
	const InputPacket inputData;
	networkInput = createManager(inputData);
}

void NetworkManager::listenForMessages()
{
	ASSERT(Error, mThread == nullptr, "Changing network manager to listening for messages, network thread is not empty and about to be overwritten\n");

	std::thread listener(listening, mServer);

	mThread = &listener;

	mThread->detach();
}


void NetworkManager::sendMessages()
{
	ASSERT(Error, mThread == nullptr, "Changing network manager to sending messages, network thread is not empty and about to be overwritten\n");

	mClient->input = input;
	mClient->frameRateController = frameRateController;

	std::thread sender(sending, mClient);

	mThread = &sender;

	mThread->detach();
}



InputPacket getClientInput(const InputManager* input)
{
	std::vector<Button> activeButtons = input->getActiveButtons();

	InputPacket inputPacket;
	if (activeButtons.size() > 0)
	{
		inputPacket.button = activeButtons[0];
	}

	inputPacket.cursor = *input->getCursor();

	return inputPacket;
}


int NetworkManager::handleNetworkInput()
{
	if (mServer)
	{
		if (mServer->mData.size() > 0)
		{
			const NetworkData& data = mServer->mData.front();

			const InputPacket inputData = *reinterpret_cast<const InputPacket*>(data.buffer);
			if (inputData.hasData)
			{
				// update the input
				networkInput = createManager(inputData);

				if (networkInput.isHeld(Button::Right))
				{
					printf("move right\n");
				}
			}

			// TODO: need to set the input manager, not reset the states, and then
			// overrite it every time there's a message
			//InputManager inputManagment = createManager(inputData);

			//netoworkPlayer->handleInput(&inputManagment);

			mServer->mData.pop();
		}


		//return mMovement;
	}
	//else
	//{
	//	return -1;
	//}


	netoworkPlayer->handleInput(&networkInput);
	return -1;
}


void listening(Server* server)
{
	printf("listening\n");

	double frameNumber = 0.0;

	while (true)
	{
		BasicString message("", 1024);
		BasicString senderInfo("", 1024);

		NetworkData data;

		server->receiveMessage(data, nullptr);

		//if (data.hasData)
		{
			server->mData.push(data);
			//printf("receieving data\n");
		}
	}
}


void sending(Client* client)
{
	long frame = client->frameRateController->frameCount();

	//printf("sending messages\n");
	while (true)
	{
		InputPacket packet = getClientInput(client->input);
		packet.hasData = true;


		NetworkData data;
		memcpy(data.buffer, (void*)&packet, sizeof(packet));

		data.size = sizeof(packet);
		data.hasData = true;

		client->sendMessage(data);
		//printf("sent\n");

		const InputPacket inputData = *reinterpret_cast<const InputPacket*>(data.buffer);


		if (inputData.button.isHeld())
		{
			printf("sending button data\n");
		}

		while (client->frameRateController->frameCount() <= frame)
		{
			Sleep(0);
		}

		frame = client->frameRateController->frameCount();
		//printf("frame count %ld\n", frame);

		//BasicString message("", 1024);
		//std::cin.get(message.buffer(), message.bufferLength());

		//message.calculateLength();
		//client->sendMessage(packet);

		//std::cin.ignore();
	}
}