#include "stdafx.h"
#include "Tray.h"


Tray::Tray(HWND hWnd, HINSTANCE hInst) {
	dummyWin = hWnd;
	hInstance = hInst;

	enabledIcon = LoadIcon(hInstance, (LPCTSTR)MAKEINTRESOURCE(IDI_TORTRAYICON_ON));
	disabledIcon = LoadIcon(hInstance, (LPCTSTR)MAKEINTRESOURCE(IDI_TORTRAYICON_OFF));

	init();
}

void Tray::init() {
	// tray icon settings
	structNID.cbSize = sizeof(NOTIFYICONDATA);
	structNID.hWnd = dummyWin;
	structNID.uID = IDI_TORTRAYICON_ON;
	structNID.uVersion = NOTIFYICON_VERSION_4;
	structNID.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO | NIF_SHOWTIP| NIF_STATE;
	wcscpy_s(structNID.szTip, L"TorTray");
	structNID.hIcon = enabledIcon;
	structNID.uCallbackMessage = WM_USER_SHELLICON;
	structNID.dwState = 0;
	structNID.dwStateMask = 0;

	// Display tray icon
	if (!Shell_NotifyIcon(NIM_ADD, &structNID)) {
		DestroyWindow(dummyWin);
	}
	Shell_NotifyIcon(NIM_SETVERSION, &structNID);

}

void Tray::updateIcon(BOOL enabled) {
	structNID.hIcon = enabled ? enabledIcon : disabledIcon;

	Shell_NotifyIcon(NIM_MODIFY, &structNID);
}

void Tray::destroy(){
	Shell_NotifyIcon(NIM_DELETE, &structNID);
	DestroyIcon(enabledIcon);
	DestroyIcon(disabledIcon);
}

Tray::~Tray()
{
	destroy();
}
