#include "../pch.h"
#include "Helpers.h"

//using namespace std;
HWND m_hWnd;
// 开始HOOK函数
void Helpers::HookFunction(PVOID* oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(oFunction, pDetour);
	DetourTransactionCommit();
}

// 结束HOOK函数
void Helpers::UnHookFunction(PVOID* oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(oFunction, pDetour);
	DetourTransactionCommit();
}

BOOL Helpers::EnumWindowsCallback(HWND hWnd, LPARAM lParam)
{
	DWORD wndPid;
	GetWindowThreadProcessId(hWnd, &wndPid);
	if (wndPid != (DWORD)lParam) return TRUE;

	m_hWnd = hWnd;
	return FALSE;
}

// 获取当前所在窗口的句柄
HWND Helpers::GetCurrentWindowHandle()
{
	m_hWnd = NULL;
	EnumWindows(EnumWindowsCallback, GetCurrentProcessID());
	return m_hWnd;
}

DWORD Helpers::GetCurrentProcessID(){return GetCurrentProcessId();}



