#include "stdafx.h"
#include "processHelper.h"

DWORD getPIDbyName(PWCHAR name) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!wcscmp(entry.szExeFile, name)) {
				CloseHandle(snapshot);
				return entry.th32ProcessID;
			}
	
	CloseHandle(snapshot);
	return 0;
}


void startProcess(PWCHAR path) {
	WCHAR buffer[1024];
	wcscpy_s(buffer, path);
	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	BOOL r = CreateProcess(NULL, buffer, NULL, NULL, FALSE, DETACHED_PROCESS | CREATE_NEW_PROCESS_GROUP, NULL, NULL, &info, &processInfo);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
}

void killProcess(DWORD pid) {
	HANDLE explorer;
	explorer = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	TerminateProcess(explorer,0);
	CloseHandle(explorer);
}

void killProcess(PWCHAR name) {
	killProcess(getPIDbyName(name));
}