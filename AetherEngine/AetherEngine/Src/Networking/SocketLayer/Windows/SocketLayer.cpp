#ifdef _WIN32

#include "Src/Networking/SocketLayer/SocketLayer.h"
#include "Src/Logging/Logger.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

namespace Aeth::Networking
{
	SocketLayer::SocketLayer()
	{
		SOCKET Sock = INVALID_SOCKET;
		WSADATA wsaData;
	}

	int SocketLayer::InitializeWinsock()
	{
		int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (res != NO_ERROR) {
			printf("WSAStartup failed with error %d\n", res);

			return 1;
		}
		return 0;
	}

	int SocketLayer::CreateSocket()
	{
		Sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (Sock == INVALID_SOCKET) {
			printf("socket failed with error %d\n", WSAGetLastError());
			return 1;
		}
		return 0;
	}

	int SocketLayer::BindSocket(short port, const char* ip)
	{
		bindAddress.sin_family = AF_INET;
		bindAddress.sin_port = htons(port);
		inet_pton(AF_INET, (PCSTR)(ip), &bindAddress.sin_addr);

		if (bind(Sock, (SOCKADDR*)&bindAddress, sizeof(bindAddress))) {
			printf("bind failed with error %d\n", WSAGetLastError());
			return 1;
		}

		return 0;
	}

	int SocketLayer::BindSocket()
	{
		bindAddress.sin_family = AF_INET;
		bindAddress.sin_port = htons(0);
		bindAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		//inet_pton(AF_INET, (PCSTR)(INADDR_ANY), &bindAddress.sin_addr);

		if (bind(Sock, (SOCKADDR*)&bindAddress, sizeof(bindAddress))) {
			printf("bind failed with error %d\n", WSAGetLastError());
			return 1;
		}

		return 0;
	}

	int SocketLayer::Listen()
	{
		char serverBuf[2049];
		int serverBufLen = 2048;
		struct sockaddr_in messageSenderAddr;
		int messageSenderAddrSize = sizeof(messageSenderAddr);

		int bytes_received = recvfrom(Sock, serverBuf, serverBufLen, 0, (SOCKADDR*)&messageSenderAddr, &messageSenderAddrSize); // 0 = no flags
		if (bytes_received == SOCKET_ERROR) {
			printf("recvfrom failed with error %d\n", WSAGetLastError());
			//Logger::Lock() << "recvfrom failed with error " << WSAGetLastError() << Logger::ENDL
		}

		return 0;
	}

	int SocketLayer::Send(char data[], sockaddr_in address)
	{
		int dataLength = (int)(sizeof(data) - 1);

		int sendResult = sendto(Sock, data, dataLength, 0, (SOCKADDR*)&address, sizeof(address));
		if (sendResult == SOCKET_ERROR) {
			printf("Send failed with error: %d\n", WSAGetLastError());
		}

		return sendResult;
	}
}

#endif
