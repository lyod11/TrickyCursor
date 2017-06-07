#include "stdafx.h"
#include "MouseHook.h"
#include "Cursor.h"

MouseHook::MouseHook()
{
}

MouseHook::~MouseHook()
{
}

MouseHook & MouseHook::getInstance()
{
	static MouseHook hook = MouseHook();
	return hook;
}

void MouseHook::setHook()
{
	hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0);
	//use assert!!!
}

void MouseHook::releaseHook()
{
	UnhookWindowsHookEx(hHook);
}

int MouseHook::loopThroughMessages()
{
	int wmId = LOWORD(message.lParam);
	while ((wmId != SWM_DISABLE) && (message.message != WM_QUIT))
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		Sleep(10);
	}
	releaseHook();
	return (int)message.wParam;
}

LRESULT WINAPI MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSLLHOOKSTRUCT * pMouseStruct = (MSLLHOOKSTRUCT *)lParam;

	if (nCode == 0) { 
		if (pMouseStruct != NULL) { 		
			switch (wParam) {

			case WM_MOUSEMOVE: {
				Cursor::getInstance().SetCustomCursor(pMouseStruct->pt);
			}break;
			}
		}
	}

	return CallNextHookEx(MouseHook::getInstance().hHook, nCode, wParam, lParam);
}
