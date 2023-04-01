#include <iostream>
#include <cstring>
#include "server.hpp"
#include "client.hpp"

// Entry point of the application
int WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance, 
            LPTSTR    lpCmdLine, 
            int       cmdShow)
{
	tcp::server();
	return 0;
}
