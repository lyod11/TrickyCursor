#pragma once

#include "Resource.h"
#include "stdafx.h"

#define DEFAULT_CURSOR 32512

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
};

