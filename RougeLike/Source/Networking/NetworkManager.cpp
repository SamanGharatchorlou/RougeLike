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


int NetworkManager::handleNetworkInput()
{
	if (mServer)
	{
		if (mServer->mInputData.size() > 0)
		{
			const NetworkInputData& networkData = mServer->mInputData.front();
			InputPacket inputData = networkData.unpack();

			networkInput = createManager(inputData);

			mServer->mInputData.pop();
		}
	}

	netoworkPlayer->handleInput(&networkInput);
	return -1;
}


void listening(Server* server)
{
	while (true)
	{
		NetworkInputData data;
		server->receiveMessage(data, nullptr);
		server->mInputData.push(data);
	}
}


void sending(Client* client)
{
	long frame = client->frameRateController->frameCount();

	while (true)
	{
		InputPacket packet(client->input);

		NetworkInputData data;
		data.pack(packet);

		client->sendMessage(data);

		while (client->frameRateController->frameCount() <= frame)
		{
			Sleep(0);
		}

		frame = client->frameRateController->frameCount();
	}
}