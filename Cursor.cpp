#include "stdafx.h"
#include "Cursor.h"


Cursor::Cursor()
{
	prevPosition.x = NULL;   
	hcDefault = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
}

Cursor & Cursor::getInstance()
{
	static Cursor cursor = Cursor();
	return cursor;
}

void Cursor::SetDefaultCursor()
{
	BOOL status = SetSystemCursor(hcDefault, DEFAULT_CURSOR);
	DestroyCursor(hcDefault);
	hcDefault = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
}

void Cursor::SetCustomCursor(POINT &coordinates)
{
	
	if (prevPosition.x == NULL) {
		prevPosition = coordinates;
		return;
	}
	int angle = getCursorAngle(coordinates);
	if (angle == -1)
		return;

	wchar_t buffer[BUFF_SIZE];
	wchar_t path[BUFF_SIZE];
	GetCurrentDirectory(BUFF_SIZE, path);
	lstrcatW(path, CURSOR_PATH);
	_itow_s(angle, buffer, 10);
	lstrcatW(path, buffer);
	lstrcatW(path, L".cur");

	/*wchar_t wtext[BUFF_SIZE];
	size_t outSize;
	mbstowcs_s(&outSize, wtext, path, strlen(path) + 1);
	LPCWSTR newPath = wtext;*/

	HCURSOR newCursor = CopyCursor(LoadCursorFromFile(path));

	SetSystemCursor(newCursor, DEFAULT_CURSOR);
	DestroyCursor(newCursor);

	prevPosition = coordinates;

	
}

void Cursor::DeleteCursor()
{
	DestroyCursor(hcDefault);
	hcDefault = NULL;
}



int Cursor::getCursorAngle(POINT &currPosition)
{
	double a = currPosition.x - prevPosition.x;
	double b = -(currPosition.y - prevPosition.y);
	if((a != 0) && (a < MEASUREMENT_ERROR) && (a > -MEASUREMENT_ERROR))
		if((b !=0) && (b < MEASUREMENT_ERROR) && (b > -MEASUREMENT_ERROR))
			return -1;

	double angle;

	if (b == 0)
		angle = 90;
	else
		angle = abs(atan(a / b))*RADIAN;

	if(a >= 0 && b < 0){ // II quarter
		angle = 180 - angle;
	} else if (a < 0 && b < 0){	// III quarter
		angle = 270 - angle;
	} else if(a < 0 && b >= 0){	// IV quarter
		angle = 360 - angle;	
	}


	for (int i = 0; i < CURSOR_NUMBERS - 1; i++)
	{
		if (angle >= avalibleCursorAngles[i] && angle < avalibleCursorAngles[i+1])
			return avalibleCursorAngles[i];
	}
	return -1;
}




