#pragma once

#include "stdafx.h"

#include "Tray.h"
#include "processHelper.h"
#include "resource.h"


// Forward declarations of functions included in this code module:
ATOM                DummyWindow(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void initApp(_In_ HINSTANCE hInstance, 
			 _In_ int       nCmdShow);

int mainLoop();

void processTrayMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void switchTor();