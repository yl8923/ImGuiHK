#pragma once

class Helpers
{
public:
	// HOOK��
	static void HookFunction(PVOID* oFunction, PVOID pDetour);
	static void UnHookFunction(PVOID* oFunction, PVOID pDetour);

	// ������Ϣ��ȡ��
	static BOOL		CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam);
	static HWND		GetCurrentWindowHandle();
	static DWORD	GetCurrentProcessID();

	

};

