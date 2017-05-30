#include "stdafx.h"
#include "LaunchHelper.h"


LaunchHelper::LaunchHelper(HINSTANCE hInstance, HWND hDlg)
{
	niData.cbSize = sizeof(NOTIFYICONDATA);
	niData.uID = TRAY_ICONID;
	niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	niData.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_TRICKYCURSOR),
		IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);
	niData.hWnd = hDlg;
	niData.uCallbackMessage = SWM_TRAYMSG;
}


LaunchHelper::~LaunchHelper()
{
}


void LaunchHelper::showTrayIcon()
{
	Shell_NotifyIcon(NIM_ADD, &niData);

	if (niData.hIcon && DestroyIcon(niData.hIcon))  //free handle
		niData.hIcon = NULL;
}

std::wstring LaunchHelper::GetSturtupFolderPath()
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
		throw std::runtime_error("The SHGetKnownFolderPath function failed");
	}
}

HRESULT LaunchHelper::CreateShortcutLink()
{

	std::wstring pathLink = this->GetSturtupFolderPath();
	TCHAR buffer[MAX_PATH];
	std::wstring exePath = std::wstring(buffer, GetModuleFileName(NULL, buffer, MAX_PATH));
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
		psl->SetPath(exePath.c_str());
		psl->SetDescription(NULL);

		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hres))
		{
			WCHAR wsz[MAX_PATH];
			hres = _wmakepath_s(wsz, _MAX_PATH, NULL, pathLink.c_str(),
				L"TrickyCursor", L"lnk");
			hres = ppf->Save(wsz, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
	return hres;
}

HRESULT LaunchHelper::RemoveStartupFile()
{
	std::wstring pathLink = this->GetSturtupFolderPath();
	WCHAR wsz[MAX_PATH];
	HRESULT hres = _wmakepath_s(wsz, _MAX_PATH, NULL, pathLink.c_str(),
		L"TrickyCursor", L"lnk");
	return DeleteFile(wsz);
}
