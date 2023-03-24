#ifndef SERVER_H
#define SERVER_H
#include <winsock2.h>
#include <ws2tcpip.h>

namespace tcp {
	// Initialize winsock
	// Create a socket
	// Bind the socket
	// Listen on the socket for a client
	// Accept a connection from a client 
	// Receive and send data
	// Disconnect
	void serverSend(SOCKET);
	bool server();
}

#endif
