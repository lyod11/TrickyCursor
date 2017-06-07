#pragma once
#include <Windows.h>
#include "Resource.h"
#include "Cursor.h"

class MouseHook
{
	Cursor cursor;
public:
	MouseHook();
	~MouseHook();
	static MouseHook& getInstance();
	HHOOK hHook;
	void setHook();
	void releaseHook();

	MSG message;
	BOOL cursorFlag = FALSE;

	int loopThroughMessages();
};

LRESULT WINAPI MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam);