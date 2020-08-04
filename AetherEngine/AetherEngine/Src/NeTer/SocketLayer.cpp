/// <summary>Creates a socket if one does not already exist. </summary>
/// <remarks>Arron Davies, 01-08-2020. </remarks>
/// <returns>Nothing(self). </returns>


#include "SocketLayer.h"
bool SocketLayer::socketStarted = false;

#ifdef _WINDOWS

#include <winsock2.h>
#include <ws2tcpip.h>
#include "Src/Logging/Logger.h"

WSADATA SocketLayer::wsaData;
addrinfo* SocketLayer::result = NULL, * ptr = NULL;
SOCKET SocketLayer::Socket = INVALID_SOCKET;

SocketLayer::SocketLayer()
{
	if (socketStarted == false)
	{
		int WSAResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (WSAResult != 0)
		{
			Logger::Log(Logger::ERROR, "WSAStartup failed: ", WSAResult);
			socketStarted = false;
			return;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		socketStarted = true;
	}
}

bool SocketLayer::Connect(std::string IP, std::string Port)
{
	int AdressInfoResult = getaddrinfo(IP.c_str(), Port.c_str(), &hints, &result);
	if (AdressInfoResult != 0)
	{
		Logger::Log(Logger::ERROR, "getaddrinfo failed: ", AdressInfoResult);
		WSACleanup();
		return false;
	}

	Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (Socket == INVALID_SOCKET) {
		Logger::Log(Logger::ERROR, "Error at socket(): ", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	int ConnectResult = connect(Socket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (ConnectResult == SOCKET_ERROR) {
		closesocket(Socket);
		Socket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (Socket == INVALID_SOCKET) {
		Logger::Log(Logger::ERROR, "Unable to connect to server!");
		WSACleanup();
		return false;
	}

	return true;
}

bool SocketLayer::PortInUse()
{

}

bool SocketLayer::Bind(std::string IP, std::string Port)
{
	int AdressInfoResult;
	if (IP == "")
	{
		AdressInfoResult = getaddrinfo(NULL, Port.c_str(), &hints, &result);
	}
	else
	{
		AdressInfoResult = getaddrinfo(IP.c_str(), Port.c_str(), &hints, &result);
	}

	if (AdressInfoResult != 0) {
		Logger::Log(Logger::ERROR,  "getaddrinfo failed: ", AdressInfoResult);
		WSACleanup();
		return false;
	}

	Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (Socket == INVALID_SOCKET) {
		Logger::Log(Logger::ERROR, "Error at socket(): ", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	int BindResult = bind(Socket, result->ai_addr, (int)result->ai_addrlen);
	if (BindResult == SOCKET_ERROR) {
		Logger::Log(Logger::ERROR, "bind failed with error: ", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(Socket);
		WSACleanup();
		return false;
	}

	return true;
}

#elif LINUX

#endif