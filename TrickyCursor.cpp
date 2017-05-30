// TrickyCursor.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "TrickyCursor.h"


HINSTANCE hInst;                                // current instance
BOOL defaultCursorFlag = TRUE;
BOOL startupLaunchFlag = FALSE;
Cursor *cursor;
LaunchHelper *launchHelper;
Menu *menu;


BOOL                InitInstance(HINSTANCE, int);


LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRICKYCURSOR));

    MSG msg;
	launchHelper->showTrayIcon();
	
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_TRICKYCURSOR_DIALOG),
	   NULL, (DLGPROC)WndProc);



   if (!hDlg)
   {
	   return FALSE;
   }

   cursor = new Cursor();
   launchHelper = new LaunchHelper(hInstance, hDlg);
   menu = new Menu();

 
   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
    switch (message)
    {
	case WM_SETCURSOR:

	case SWM_TRAYMSG:
		switch (lParam)
		{
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			menu->ShowContextMenu(hWnd, defaultCursorFlag, startupLaunchFlag);
			break;
		}

    case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case SWM_DISABLE:
			
			cursor->SetDefaultCursor();
			defaultCursorFlag = TRUE;

			break;
		case SWM_ENABLE:
			cursor->SetCustomCursor();
			defaultCursorFlag = FALSE;

			break;
		case SWM_LAUNCH_CHECKED:
			launchHelper->RemoveStartupFile();
			startupLaunchFlag = FALSE;
			break;

		case SWM_LAUNCH_UNCHECKED:
			launchHelper->CreateShortcutLink();
			startupLaunchFlag = TRUE;
			break;
		case SWM_EXIT:
			if (defaultCursorFlag == false)
			{
				cursor->SetDefaultCursor();
				cursor->DeleteCursor();
				defaultCursorFlag = TRUE;
			}
			delete cursor;
			delete launchHelper;
			delete menu;
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
		}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
