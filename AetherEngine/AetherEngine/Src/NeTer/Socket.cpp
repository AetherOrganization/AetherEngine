/// <summary>Creates a socket</summary>
/// <remarks>Arron Davies, 06-08-2020. </remarks>

#include "Socket.h"

#ifdef _WINDOWS
bool Socket::SocketStarted = false;
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Src/Logging/Logger.h"

Socket::Socket()
{
	if (SocketStarted == false)
	{
		WSADATA wsaData;

		// Initialize Winsock
		int WSAResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (WSAResult != 0) {
			Logger::Log(Logger::ERROR, "WSAStartup failed: ", WSAResult);
			return;
		}

		SOCKET Sock = INVALID_SOCKET;
		Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (Sock == INVALID_SOCKET) {
			Logger::Log(Logger::ERROR, "Socket failed with error: ", WSAGetLastError());
			return;
		}
	}

	SocketStarted = true;
}

int Socket::Bind(int Port, const char* IP)
{
	struct sockaddr_in ServerAddress;

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(Port);
	InetPton(AF_INET, (PCWSTR)(IP), &ServerAddress.sin_addr.s_addr);

	if (bind(Sock, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress)))
	{
		Logger::Log(Logger::ERROR, "bind failed with error: ", WSAGetLastError());
		return 1;
	}
}

int Socket::Listen()
{
	int bytes_received;
	char serverBuf[1537];

	struct sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);

	bytes_received = recvfrom(Sock, serverBuf, 1536, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize); /*Socket, buffer, buffer size, flags, sender address struct, sender address size*/
	if (bytes_received == SOCKET_ERROR) {
		Logger::Log(Logger::ERROR, "recvfrom failed with error: ", WSAGetLastError());
	}
	serverBuf[bytes_received] = '\0';
}

#endif