// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H
#define WIN32_LEAN_AND_MEAN  

// 添加要在此处预编译的标头
#include <stdio.h>
#include <io.h>

#include <Windows.h>
#include <tchar.h>
#include <winnls.h>
#include <iostream>
#include <WinDNS.h>
#include <chrono>
#include <cstdio>
#include <cassert>

#include <d3d11.h>
#include <D3Dcompiler.h>
//#include <libop.h>
#include <atlstr.h>


// ImGui头文件载入
#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imstb_rectpack.h"
#include "ImGui/imstb_textedit.h"
#include "ImGui/imstb_truetype.h"

// 自创字体文件
#include "ImGui/baidu_font.hpp"
//#include "./sytpe.hpp"

#include "Detours/include/detours.h"

#include "SDK/Helpers.h"
#include "SDK/string_util.h"

#include <vector>
#include <string>



#endif //PCH_H


//  部分函数指针类型声明
typedef HRESULT(__stdcall* tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SyscInterval, UINT Flags);
typedef LRESULT(__stdcall* WndProc_t)(const HWND, UINT, WPARAM, LPARAM);
typedef HRESULT(__stdcall* ResizeBuffers)(IDXGISwapChain* This, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);





