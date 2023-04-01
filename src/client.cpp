#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <cstring>
#include "server.hpp"
#include "client.hpp"

bool status = true;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		case CTRL_C_EVENT:
			status = false;
			return TRUE;
		case CTRL_CLOSE_EVENT:
			status = false;
			return TRUE;
		default:
			return FALSE;
	}
}

// Entry point of the application
int WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance, 
            LPTSTR    lpCmdLine, 
            int       cmdShow)
{
	if (__argc != 2) 
	{
		std::cerr << "Pass an IPv4 address." << std::endl;
		return 1;
	}
	SetConsoleCtrlHandler(CtrlHandler, TRUE);
	tcp::client(__argv[1], "12345", status);
	return 0;
}
