#pragma once
#include <string>

class Socket
{
public:
	Socket();
	int Bind(int Port, const char* IP);
	int Listen();
	static bool SocketStarted;
private:
#ifdef _WINDOWS
	SOCKET Sock;
#endif
};