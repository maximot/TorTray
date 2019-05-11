#pragma once

#include "stdafx.h"

DWORD getPIDbyName(PWCHAR name);

void startProcess(PWCHAR path);

void killProcess(DWORD pid);
void killProcess(PWCHAR name);