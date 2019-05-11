// TorTray.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TorTray.h"

#define MAX_LOADSTRING 255

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

Tray* tray;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	initApp(hInstance, nCmdShow);

	return mainLoop();

}

int mainLoop() {
	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		DWORD torPid = getPIDbyName(L"tor.exe");
		if (torPid) {
			tray->updateIcon(TRUE);
		} else {
			tray->updateIcon(FALSE);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	delete tray;
	return (int)msg.wParam;
}

void initApp(_In_ HINSTANCE hInstance,
			 _In_ int       nCmdShow) 
{
	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TORTRAY, szWindowClass, MAX_LOADSTRING);
	DummyWindow(hInstance);

	// Perform application initialization:
	HWND hWnd = InitInstance(hInstance, nCmdShow);

	if(hWnd == 0) {
		exit(0);
	}

	SetTimer(hWnd,             // handle to main window 
		0,            // timer identifier 
		1000,                 
		(TIMERPROC)NULL);

	tray = new Tray(hWnd, hInstance);

}



//
//  FUNCTION: DummyWindow()
//
//  PURPOSE: Registers the window class.
//
ATOM DummyWindow(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW; 
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TORTRAYICON_ON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_TORTRAYICON_ON));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

   return hWnd;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_USER_SHELLICON:
		processTrayMessage(hWnd, message, wParam, lParam);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void processTrayMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	POINT lpClickPoint;

	switch (LOWORD(lParam))
	{
	case WM_RBUTTONDOWN:
	{
		HMENU hMenu;
		HMENU hSub;
		// get mouse cursor position x and y as lParam has the Message itself
		GetCursorPos(&lpClickPoint);

		// Load menu resource
		hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_TORTRAY));
		if (!hMenu)
			return;

		hSub = GetSubMenu(hMenu, 0);
		
		if (!hSub) 
			return;


		// Display menu
		SetForegroundWindow(hWnd);
		TrackPopupMenu(hSub, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
		SendMessage(hWnd, WM_NULL, 0, 0);

		// Kill off objects we're done with
		DestroyMenu(hMenu);
		break;
	}
	case WM_LBUTTONDOWN:
		switchTor();
	break;
	}
}

void switchTor() {
	DWORD torPid = getPIDbyName(L"tor.exe");
	if (torPid) {
		killProcess(torPid);
	} else {
		startProcess(L"tor.exe");
	}
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
