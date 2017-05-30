#include "stdafx.h"
#include "Cursor.h"


Cursor::Cursor()
{
	hcDefault = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	hNewCursor = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));
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

Cursor::~Cursor()
{
}
