#include "server.hpp"
#include "winsock2.h"
#include "ws2tcpip.h"
#include <memory>
#include <iostream>
#include <thread>
#include <vector>
#include <string>

namespace tcp 
{
	void serverSend(SOCKET ClientSocket) 
	{
		std::cout << "Client connected: " << &ClientSocket << std::endl;
		char sendbuffer[512] = "data delivered as requested", recvbuffer[512];
		int irecvResult, isendResult, iResult;
		while (true) {
			irecvResult = recv(ClientSocket, recvbuffer, sizeof(recvbuffer), 0);
			if (irecvResult > 0)
			{
				isendResult = send(ClientSocket, sendbuffer, sizeof(sendbuffer), 0);
				if (isendResult == SOCKET_ERROR) 
				{
					std::cout << "send failed: " << WSAGetLastError() << "\n";
					closesocket(ClientSocket);
					break;
				}
			}
			else if (irecvResult == 0)
			{
				std::cout << "Client disconnected.\n";
				closesocket(ClientSocket);
				break;
			}
			else
			{
				std::cout << "recv failed: " << WSAGetLastError() << "\n";
				closesocket(ClientSocket);
				break;
			}
			Sleep(10);
		}
	}

	bool server() 
	{
		std::cout << "Server started" << std::endl;

		// Initialize winsock
		struct addrinfo *result = NULL;
		struct addrinfo *ptr = NULL;
		struct addrinfo hints;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		WSAData wsadata;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);

		if (iResult != 0) 
		{
			std::cerr << "WSAStartup failed with error: " << iResult << "\n";
			WSACleanup();
			return false;
		}

		// Resolve the local address and port to be used by the server
		iResult = getaddrinfo(NULL, "12345", &hints, &result);

		if (iResult != 0) 
		{
			std::cerr << "getaddrinfo failed: " << iResult << "\n";
			return false;
		}

		// Create a socket
		SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		if (ListenSocket == INVALID_SOCKET) 
		{
			std::cerr << "Error at socket(): " << WSAGetLastError() << "\n";
			freeaddrinfo(result);
			WSACleanup();
			return false;
		}

		// Bind the socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			std::cerr << "bind failed with error: " << WSAGetLastError() << "\n";
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			return false;
		}

		std::cout << "i'm waiting here.." << std::endl;

		// Listen on the socket for a client
		iResult = listen(ListenSocket, SOMAXCONN);

		if (iResult == SOCKET_ERROR)
		{
			std::cerr << "Listen failed with error: " << WSAGetLastError() << "\n";
			closesocket(ListenSocket);
			WSACleanup();
			return false;
		}

		// Accept connection from clients
		while (true)
		{

			SOCKET ClientSocket = INVALID_SOCKET;

			std::cout << "Waiting for an incoming client..." << std::endl;
			ClientSocket = accept(ListenSocket, NULL, NULL);

			// Receive and send data
			std::thread worker(sendDataFunc, ClientSocket);	
			worker.detach();

		}

		// Disconnect
		closesocket(ListenSocket);
		WSACleanup();
		
		return true;
	}
}
