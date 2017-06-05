#pragma once

#include "Resource.h"
#include "stdafx.h"
//#include <Gdiplusgraphics.h>


#define DEFAULT_CURSOR 32512
#define _AFXDLL

class Cursor
{
	HCURSOR hcDefault;
	HCURSOR hNewCursor;

public:
	Cursor();
	~Cursor();

	void SetDefaultCursor();
	void SetCustomCursor();
	void DeleteCursor();
	void rotateCursor();
	void rotateBitmap(HBITMAP &bmp, HPALETTE plt, int angle);

};

