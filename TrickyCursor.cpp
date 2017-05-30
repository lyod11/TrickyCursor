// TrickyCursor.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TrickyCursor.h"



#define MAX_LOADSTRING 100
#define TRAYICONID	1//				ID number for the Notify Icon
#define SWM_TRAYMSG	WM_APP//		the message ID sent to our window


#define SWM_ENABLE	WM_APP + 1	
#define SWM_DISABLE	WM_APP + 2	
#define SWM_LAUNCH	WM_APP + 3
#define SWM_EXIT	WM_APP + 4

// Global Variables:
HINSTANCE hInst;                                // current instance
NOTIFYICONDATA niData;	//for system tray icon
HCURSOR hcDefault;
HCURSOR hNewCursor;
BOOL defaultCursorFlag = TRUE;


// Forward declarations of functions included in this code module:
//ATOM                MyRegisterClass(HINSTANCE hInstance);




BOOL                InitInstance(HINSTANCE, int);
void				ShowContextMenu(HWND hWnd);


LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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

	hcDefault = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	hNewCursor = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));



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

  
   niData.cbSize = sizeof(NOTIFYICONDATA);
   niData.uID = TRAYICONID;
   niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   niData.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_TRICKYCURSOR),
	   IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
	   LR_DEFAULTCOLOR);
   niData.hWnd = hDlg;
   niData.uCallbackMessage = SWM_TRAYMSG;

   Shell_NotifyIcon(NIM_ADD, &niData);

   if (niData.hIcon && DestroyIcon(niData.hIcon))  //free handle
	   niData.hIcon = NULL;

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

void ShowContextMenu(HWND hDlg)
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
		InsertMenu(hSubMenu, -1, MF_BYPOSITION, SWM_LAUNCH, _T("Launch at Windows Login"));
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
    switch (message)
    {
	case WM_SETCURSOR:
	
		// If we're the control that should get the cursor treatment...
			//SetCursor(hcCross);
			return TRUE;  // indicate we processed this message

	case SWM_TRAYMSG:
		switch (lParam)
		{
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			ShowContextMenu(hWnd);
		}

    case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case SWM_DISABLE:
			SetSystemCursor(hcDefault, 32512);
			DestroyCursor(hcDefault);
			hcDefault = NULL;
			defaultCursorFlag = TRUE;

			break;
		case SWM_ENABLE:
			
			SetSystemCursor(hNewCursor, 32512);
			defaultCursorFlag = FALSE;
			/*DestroyCursor(hNewCursor);
			hcDefault = NULL;*/

			break;
		case SWM_LAUNCH:

			break;
		case SWM_EXIT:
			DestroyWindow(hWnd);
			break;
		}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
/*
void AddToStartup(string name)
{
	RegistryKey rkApp = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
	rkApp.SetValue("TrickyCursor.exe", Application.ExecutablePath.ToString());
}
*/
