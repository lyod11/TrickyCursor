#pragma once

#include "stdafx.h"
#include "Resource.h"

class Menu
{
public:
	Menu();
	~Menu();

	void ShowContextMenu(HWND hDlg, BOOL defaultCursorFlag, BOOL startupLaunchFlag);
};

