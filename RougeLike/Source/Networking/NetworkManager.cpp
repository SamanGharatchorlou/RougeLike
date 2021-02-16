#include "pch.h"
#include "NetworkManager.h"


void NetworkManager::listenForMessages()
{
	//std::thread listener(listening, mServer);

	mThread = new std::thread(listening, mServer); //&listener;

	mThread->detach();
}


void NetworkManager::sendMessages()
{
	printf("sending messages\n");
	while (true)
	{
		BasicString message("", 1024);
		std::cin.get(message.buffer(), message.bufferLength());

		message.calculateLength();
		mClient->sendMessage(message);

		std::cin.ignore();
	}
}


void listening(Server* server)
{
	printf("listening\n");
	while (true)
	{
		BasicString message("", 1024);
		BasicString senderInfo("", 1024);

		server->receiveMessage(message, nullptr);

		if (!message.empty())
		{
			DebugPrint(Log, "%s\n", message.c_str());
		}
	}
}