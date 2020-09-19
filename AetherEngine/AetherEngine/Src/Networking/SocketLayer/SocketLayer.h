#pragma once


#ifdef _WIN32
#include <WinSock2.h>

namespace Aeth::Networking
{
	class SocketLayer
	{
	public:
		SocketLayer();
		int InitializeWinsock();
		int CreateSocket();
		int BindSocket(short, const char*);
		int BindSocket();
		int Listen();
		int Send(char data[], sockaddr_in address);

	private:
		SOCKET Sock = INVALID_SOCKET;
		WSADATA wsaData;
		sockaddr_in bindAddress;
	};
}
#endif
