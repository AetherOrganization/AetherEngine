#pragma once
#include <string>

class SocketLayer
{
#ifdef _WINDOWS
public:
	SocketLayer();
	bool Connect(std::string IP, std::string Port);
	bool PortInUse();
	bool Bind(std::string IP, std::string Port);
	static SOCKET Socket;

private:
	static bool socketStarted;

	static WSADATA wsaData;
	static struct addrinfo* result, * ptr, hints;
#elif _LINUX

#endif
};