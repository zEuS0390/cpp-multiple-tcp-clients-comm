#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <memory>

bool server() 
{
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
	}


	// Listen on the socket for a client
	iResult = listen(ListenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "Listen failed with error: " << WSAGetLastError() << "\n";
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	/*
	std::thread keyloggerWorker(keylogger::process);
	keyloggerWorker.detach();
	*/

	// Accept connection from clients
	while (true)
	{

		SOCKET ClientSocket = INVALID_SOCKET;

		std::cout << "Waiting for an incoming client..." << std::endl;
		ClientSocket = accept(ListenSocket, NULL, NULL);

		/*
		// Receive and send data
		std::thread worker(serverSend, ClientSocket);	
		worker.detach();
		*/

	}

	// Disconnect
	closesocket(ListenSocket);
	WSACleanup();
	
	return true;
}

void client()
{

}

// Main function
int main (int argc, char** argv)
{
	/*
	if (argc != 2)
	{
		std::cerr << "Input file.\n";
		return 1;
	}
	std::cout << "Done.\n";

	const char* name = "Zeus James Baltazar\nWhat are you gonna do now?";
	int sz = 100;
	char x[sz+1];

	std::istringstream stream(name);
	std::ostringstream result;

	memset(x, 0, sz+1);
	while (!stream.eof())
	{
		stream.read(x, (int)(std::strlen(name)/4));
		int bytes = stream.gcount();
		std::cout << "Characters extracted: " << bytes << std::endl;
		std::cout << "Current position: " << stream.tellg() << std::endl;
		int n = 0;
		while (n < sz+1)
		{
			if (x[n] != 0)
			{
				result << x[n];
				std::cout << x[n];
			}
			++n;
		}
		std::cout << std::endl;
		memset(x, 0, sz+1);
	}

	std::cout << result.str() << std::endl;

	*/

	const char* filename = "897160.jpg";
	
	std::ifstream file(filename, std::ios_base::ate | std::ios_base::binary);

	int filesize = file.tellg();

	std::cout << filesize << std::endl;

	file.seekg(0);

	int buffersize = filesize / 3;

	char buffer[buffersize];

	int n = 0;
	int rs = 0;

	while (rs != 0 && n < filesize) 
	{
		file.read(buffer, buffersize);
		rs = std::strlen(buffer);
		std::cout << rs << " -> " << file.tellg() << std::endl;
		n += rs;
		memset(buffer, 0, buffersize);
		Sleep(1000);
	}

	return 0;
}
