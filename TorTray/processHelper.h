#pragma once

#include "stdafx.h"

BOOL checkIfRunning(HINSTANCE hInstance, WCHAR* windowClassName);

DWORD getPIDbyName(PWCHAR name);

void startProcess(PWCHAR path);

void killProcess(DWORD pid);
void killProcess(PWCHAR name);