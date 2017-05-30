#include "stdafx.h"
#include "Menu.h"


Menu::Menu()
{
}


Menu::~Menu()
{
}

void Menu::ShowContextMenu(HWND hDlg, BOOL defaultCursorFlag, BOOL startupLaunchFlag)
{
	POINT pt;
	GetCursorPos(&pt);

	HMENU hMenu = CreatePopupMenu();
	HMENU hSubMenu = CreatePopupMenu();
	if (hMenu)
	{
		//if default cursor
		if (defaultCursorFlag == TRUE)
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_ENABLE, _T("Enable"));
		else
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_DISABLE, _T("Disable"));
		AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, _T("Settings"));
		if (startupLaunchFlag == FALSE)
			InsertMenu(hSubMenu, -1, MF_BYPOSITION, SWM_LAUNCH_UNCHECKED, _T("Launch at Windows Login"));
		else
			InsertMenu(hSubMenu, -1, MF_BYPOSITION | MF_CHECKED, SWM_LAUNCH_CHECKED, _T("Launch at Windows Login"));

		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, _T("Exit"));


		// note:	must set window to the foreground or the
		//			menu won't disappear when it should

		SetForegroundWindow(hDlg);

		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN,
			pt.x, pt.y, 0, hDlg, NULL);
		DestroyMenu(hMenu);
		DestroyMenu(hSubMenu);
	}
}
