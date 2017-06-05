// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//



#pragma once

#include "targetver.h"

#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <afxwin.h>
#include <windows.h>
#include <stdlib.h>
#include <objidl.h>
#include <Gdiplus.h>
//using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")
//#include <Shellapi.h>
#include <Shlobj.h>
#include <iostream>
#include <iomanip>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>



