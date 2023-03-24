#include <iostream>
#include <cstring>
#include "server.hpp"
#include "client.hpp"

// Entry point of the application
int main (int argc, char **argv) 
{
	if (argc != 2) 
	{
		std::cerr << "Pass a mode (server, client)." << std::endl;
		return 1;
	}

	if (std::strcmp(argv[1], "server") == 0)
	{
		tcp::server();
	}
	else if (std::strcmp(argv[1], "client") == 0)
	{
		tcp::client();
	}
	else
	{
		std::cerr << "Invalid. Pass a mode (server, client)." << std::endl;
		return 1;
	}
	return 0;
}
