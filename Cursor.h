#pragma once

#include "Resource.h"
#include "stdafx.h"

#include <cmath>
#include <stdlib.h>

#define CURSOR_NUMBERS 13
#define CURSOR_PATH L"\\cursor_templates\\"
#define BUFF_SIZE 200
#define RADIAN 57.295
#define MEASUREMENT_ERROR 30


#define DEFAULT_CURSOR 32512
#define _AFXDLL

class Cursor
{
	HCURSOR hcDefault;
	HCURSOR hNewCursor;

	
	POINT prevPosition;
	int currentAngle;
	const int avalibleCursorAngles[CURSOR_NUMBERS] = { 0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360 };

public:

	static Cursor &getInstance();
	Cursor();
	void SetDefaultCursor();
	void SetCustomCursor(POINT &coordinates);
	void DeleteCursor();
	void rotateCursor();


private:
	int getCursorAngle(POINT &currPosition);
};

