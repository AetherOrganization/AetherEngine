#ifdef _WIN32

#include <Src/Networking/Client/Client.h>
#include <Src/Networking/SocketLayer/SocketLayer.h>
#include "Src/Logging/Logger.h"

namespace Aeth::Networking
{
	Client::Client()
	{
		Socket->InitializeWinsock();
		Socket->CreateSocket();
		Socket->BindSocket();

		std::thread listenThread([this] { this->Listen(); });

		listenThread.detach();
	}

	void Client::Listen()
	{
		while (true)
		{
			Socket->Listen();
		}
	}

	void Client::Send(void* structure)
	{

	}
}

#endif