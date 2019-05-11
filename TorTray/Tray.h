#pragma once

#include "stdafx.h"
#include "resource.h"


class Tray
{
private:
	HWND dummyWin;

	HINSTANCE hInstance;

	NOTIFYICONDATA structNID;

	HICON enabledIcon;
	HICON disabledIcon;

	void init();

	void destroy();
public:
	Tray(HWND hWnd, HINSTANCE hInstance);

	void updateIcon(BOOL enabled);

	~Tray();
};

