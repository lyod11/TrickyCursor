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


BOOL                InitInstance(HINSTANCE, int);
void				ShowContextMenu(HWND hWnd);
std::wstring		GetSturtupFolderPath();
HRESULT				CreateLink(std::wstring pathObj, std::wstring pathLink, LPCWSTR lpszDesc);
HRESULT				ResolveIt(HWND hwnd, std::wstring linkFile, std::wstring filePath);

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

	TCHAR buffer[MAX_PATH];
	std::wstring startupPath = GetSturtupFolderPath();
	std::wstring exePath = std::wstring(buffer, GetModuleFileName(NULL, buffer, MAX_PATH));

	//HRESULT res = CreateLink(exePath, startupPath, NULL);
	HRESULT res = ResolveIt(NULL, startupPath, exePath);
	
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

   hcDefault = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
   hNewCursor = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));

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
			//DestroyWindow(hWnd);
			break;
		}
    case WM_DESTROY:
      //  PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


std::wstring GetSturtupFolderPath()
{
	PWSTR pszPath;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &pszPath);
	if (SUCCEEDED(hr))
	{
		// The function succeeded, so copy the returned path to a
		// C++ string, free the memory allocated by the function,
		// and return the path string.
		std::wstring path(pszPath);
		CoTaskMemFree(static_cast<LPVOID>(pszPath));
		return path;
	}
	else
	{
		// The function failed, so handle the error.
		// ...
		// You might want to throw an exception, or just return an
		// empty string here.
		throw std::runtime_error("The SHGetKnownFolderPath function failed");
	}
}

// code from msdn
HRESULT CreateLink(std::wstring pathObj, std::wstring pathLink, LPCWSTR lpszDesc)
{
	HRESULT hres;
	IShellLink* psl;
	CoInitialize(nullptr);
	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		// Set the path to the shortcut target and add the description. 
		psl->SetPath(pathObj.c_str());
		psl->SetDescription(lpszDesc);

		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hres))
		{
			//WCHAR wsz[MAX_PATH];

			// Ensure that the string is Unicode. 
			//MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

			//const WCHAR * wsz;
			WCHAR wsz[MAX_PATH];

			// Add code here to check return value from MultiByteWideChar 
			// for success.

			// Save the link by calling IPersistFile::Save. 
			hres = _wmakepath_s(wsz, _MAX_PATH, NULL, pathLink.c_str(),
				L"MediaMaestro", L"lnk");
			hres = ppf->Save(wsz, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
	return hres;
}


// ResolveIt - Uses the Shell's IShellLink and IPersistFile interfaces 
//             to retrieve the path and description from an existing shortcut. 
//
// Returns the result of calling the member functions of the interfaces. 
//
// Parameters:
// hwnd         - A handle to the parent window. The Shell uses this window to 
//                display a dialog box if it needs to prompt the user for more 
//                information while resolving the link.
// lpszLinkFile - Address of a buffer that contains the path of the link,
//                including the file name.
// lpszPath     - Address of a buffer that receives the path of the link target, including the file name.
// lpszDesc     - Address of a buffer that receives the description of the 
//                Shell link, stored in the Comment field of the link
//                properties.



HRESULT ResolveIt(HWND hwnd, std::wstring linkFile, std::wstring filePath)
{
	HRESULT hres;
	IShellLink* psl;
	WCHAR szGotPath[MAX_PATH];
	WCHAR szDescription[MAX_PATH];
	WIN32_FIND_DATA wfd;

	CoInitialize(nullptr);

	//filePath = 0; // Assume failure 
//	filePath.clear;

				   // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
				   // has already been called. 
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		// Get a pointer to the IPersistFile interface. 
		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);

		if (SUCCEEDED(hres))
		{
			WCHAR wsz[MAX_PATH];

			// Ensure that the string is Unicode. 
			hres = _wmakepath_s(wsz, _MAX_PATH, NULL, linkFile.c_str(),
				L"MediaMaestro", L"lnk");

			// Add code here to check return value from MultiByteWideChar 
			// for success.

			// Load the shortcut. 
			hres = ppf->Load(wsz, STGM_READ);

			// Add code here to check return value from MultiByteWideChar 
			// for success.

			if (SUCCEEDED(hres))
			{
				// Resolve the link. 
				hres = psl->Resolve(hwnd, 0);

				if (SUCCEEDED(hres))
				{
					// Get the path to the link target. 
					hres = psl->GetPath(szGotPath, MAX_PATH, (WIN32_FIND_DATA*)&wfd, SLGP_SHORTPATH);

					if (SUCCEEDED(hres))
					{
						// Get the description of the target. 
						hres = psl->GetDescription(szDescription, MAX_PATH);
					}
				}
			}
			// Release the pointer to the IPersistFile interface. 
			ppf->Release();
		}

		// Release the pointer to the IShellLink interface. 
		psl->Release();
	}
	return hres;
}