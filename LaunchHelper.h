#pragma once
#include "stdafx.h"
#include "Resource.h"

#define TRAY_ICONID	1//				ID number for the Notify Icon
#define SWM_TRAYMSG	WM_APP//		the message ID sent to our window


class LaunchHelper
{
	NOTIFYICONDATA niData;	//for system tray icon
public:
	LaunchHelper(HINSTANCE hInstance, HWND hDlg);
	~LaunchHelper();

	
	void				showTrayIcon();
	std::wstring		GetSturtupFolderPath();
	HRESULT				CreateShortcutLink();
	HRESULT				RemoveStartupFile();
};

