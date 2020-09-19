#ifdef _WIN32

#include <Src/Networking/Server/Server.h>
#include <Src/Networking/SocketLayer/SocketLayer.h>
#include "Src/Logging/Logger.h"

namespace Aeth::Networking
{
	Server::Server(short port, const char* ip)
	{
		Socket->InitializeWinsock();
		Socket->CreateSocket();
		Socket->BindSocket(port, ip);

		std::thread listenThread([this] { this->Listen(); });

		listenThread.detach();
	}

	void Server::Listen()
	{
		while (true)
		{
			Socket->Listen();
		}
	}

	void Server::Send(void* structure)
	{

	}
}

#endif