#include "client.hpp"
#include "winsock2.h"
#include "ws2tcpip.h"
#include <memory>
#include <iostream>

namespace tcp 
{
	bool client() 
	{
		// Initialize winsock
		struct addrinfo *result = NULL;
		struct addrinfo *ptr = NULL;
		struct addrinfo hints; 

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		WSAData wsadata;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);

		if (iResult != 0) 
		{
			std::cerr << "WSAStartup failed with error: " << iResult << "\n";
			WSACleanup();
			return false;
		}

		// Resolve the server address and port
		iResult = getaddrinfo("localhost", "12345", &hints, &result);

		if (iResult != 0) 
		{
			std::cerr << "getaddrinfo failed: " << iResult << "\n";
			WSACleanup();
			return false;
		}
		
		// Create a socket
		SOCKET ConnectSocket = INVALID_SOCKET;

		// Attempt to connect the first address returned by the call to getaddrinfo
		ptr = result;

		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET)
		{
			std::cerr << "Error at socket(): " << WSAGetLastError() << "\n";
			freeaddrinfo(result);
			WSACleanup();
			return false;
		}

		// Connect to a server
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		
		if (iResult == SOCKET_ERROR)
		{
			std::cerr << "Unable to connect to server!\n";
			closesocket(ConnectSocket);
			WSACleanup();
			return false;
		}

		std::cout << "Successfully connected" << std::endl;

		// Send and receive data
		char sendbuffer[512] = "requesting for data", recvbuffer[512]; 
		int irecvResult, isendResult;
		while (true) {

			// Send an initial buffer to the server
			isendResult = send(ConnectSocket, sendbuffer, sizeof(sendbuffer), 0);

			if (isendResult == SOCKET_ERROR)
			{
				std::cout << "Send failed: " << WSAGetLastError() << "\n";
				break;
			}

			irecvResult = recv(ConnectSocket, recvbuffer, sizeof(recvbuffer), 0);
			if (irecvResult > 0)
			{
				std::cout << "Received message from server: ";
				std::cout << recvbuffer << std::endl;
			}
			Sleep(10);
		}

		// Disconnect
		closesocket(ConnectSocket);
		WSACleanup();

		return true;
	}
}
