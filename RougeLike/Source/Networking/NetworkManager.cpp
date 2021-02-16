#include "pch.h"
#include "NetworkManager.h"


void NetworkManager::listenForMessages()
{
	std::thread listener(listening, mServer);

	mThread = &listener;

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


int NetworkManager::handleNetworkInput()
{
	if (mServer)
	{
		if (mServer->mInputs.size() > 0)
		{
			//
			const BasicString inputString = mServer->mInputs.pop();

			if (inputString == BasicString("a"))
			{
				mMovement = 1;
			}

			if (inputString == BasicString("d"))
			{
				mMovement = 2;
			}

			if (inputString == BasicString("w"))
			{
				mMovement = 3;
			}

			if (inputString == BasicString("s"))
			{
				mMovement = 4;
			}

			if (inputString == BasicString("q"))
			{
				mMovement == 0;
			}


			//mServer->mInputs.pop();
		}


		return mMovement;
	}
	else
	{
		return -1;
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


			if (message == BasicString("a") || message == BasicString("d") || 
				message == BasicString("w") || message == BasicString("s") ||
				message == BasicString("q"))
			{
				printf("adding message %s\n", message);
				server->mInputs.push(BasicString(message));
			}
		}
	}
}