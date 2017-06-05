#include "stdafx.h"
#include "Cursor.h"


Cursor::Cursor()
{
	hcDefault = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	hNewCursor = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));
}

Cursor::~Cursor()
{
}

void Cursor::SetDefaultCursor()
{
	SetSystemCursor(hcDefault, DEFAULT_CURSOR);
}

void Cursor::SetCustomCursor()
{
	hNewCursor = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));
	hcDefault = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	SetSystemCursor(hNewCursor, DEFAULT_CURSOR);

	DestroyCursor(hNewCursor);
	hNewCursor = NULL;
}

void Cursor::DeleteCursor()
{
	DestroyCursor(hcDefault);
	hcDefault = NULL;
}


void Cursor::rotateCursor()
{
	int angle = 130; // TODO: calculate 

	HCURSOR testCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS));
	ICONINFO cInfo;
	GetIconInfo(testCursor, &cInfo);

	HBITMAP hbtm = cInfo.hbmMask;

	// Initialize GDI+.
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	rotateBitmap(hbtm, NULL, angle); //PALETTE??
	cInfo.hbmMask = hbtm;

	HCURSOR cusor = CreateIconIndirect(&cInfo);
	SetSystemCursor(cusor, DEFAULT_CURSOR);
	DestroyCursor(cusor);
	cusor = NULL;
	
}



void Cursor::rotateBitmap(HBITMAP &hbtm, HPALETTE plt, int angle) {

	Gdiplus::Bitmap *cursorBmp = new  Gdiplus::Bitmap(hbtm, plt);
	Gdiplus::Graphics *graph = new Gdiplus::Graphics(cursorBmp);

	graph->RotateTransform(angle);

	cursorBmp->GetHBITMAP(Gdiplus::Color::Black, &hbtm); //hbmp = rotated bitmap

}


