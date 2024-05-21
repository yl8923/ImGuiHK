#pragma once

class Helpers
{
public:
	// HOOK类
	static void HookFunction(PVOID* oFunction, PVOID pDetour);
	static void UnHookFunction(PVOID* oFunction, PVOID pDetour);

	// 窗口信息获取类
	static BOOL		CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam);
	static HWND		GetCurrentWindowHandle();
	static DWORD	GetCurrentProcessID();

	

};

