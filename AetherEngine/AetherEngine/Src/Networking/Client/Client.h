#pragma once

#ifdef _WIN32
#include <Src/Networking/SocketLayer/SocketLayer.h>

namespace Aeth::Networking
{
	class Client
	{
	public:
		Client();
	private:
		SocketLayer* Socket = new SocketLayer();
		void Listen();
		void Send(void* structure);
	};
}
#endif