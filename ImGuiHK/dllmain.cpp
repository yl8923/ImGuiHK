// dllmain.cpp : 定义 DLL 应用程序的入口点。
// 本页面代码为固定写法，不需要改动。
#include "pch.h"
#include "Base.hpp"
#include "engine.h"
#include "Game.h"

HWND                    g_hWnd = NULL;
HMODULE                 hOPMOUDULE;


WndProc_t               oWndProc;
tD3D11Present           oPresent;
ResizeBuffers           oResizeBuffers;


ID3D11Device*           pDevice;
ID3D11DeviceContext*    pContext;
IDXGISwapChain*         pSwapChain;
ID3D11RenderTargetView* pRenderTargetView;
HHOOK                   g_HookRet;



bool mainBeginSate  = TRUE;
bool bMouseCheck    = FALSE;
//bool tab_bar_flags = TRUE;

#ifdef _WIN64
#define SELF_PTR LONG_PTR
#define WNDPROC_INDEX GWLP_WNDPROC
#else
#define SELF_PTR int
#define WNDPROC_INDEX GWL_WNDPROC
#endif


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return true;
    
    // 拦截鼠标消息不发送到游戏窗口
    //switch (uMsg)
    //{
    //case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
    //case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
    //case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
    //case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
    //case WM_MOUSEWHEEL: case WM_MOUSEHWHEEL:
    //    //if (b鼠标消息影响游戏) return true;
    //}

    // 我们自己的消息类型处理
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK hkWndProc_(int nCode, WPARAM wParam, LPARAM lparam)
{
    CWPSTRUCT* lpArg = (CWPSTRUCT*)lparam;//结构  hwnd message wParam lParam

    return CallNextHookEx(g_HookRet, nCode, wParam, lparam);
}

HRESULT __stdcall hkResizeBuffers(IDXGISwapChain* This, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    //解决窗口缩放的时候 游戏窗口崩溃的问题
    if (pRenderTargetView) {
        pContext->OMSetRenderTargets(0, 0, 0);
        pRenderTargetView->Release();
    }

    HRESULT hr = oResizeBuffers(This, BufferCount, Width, Height, NewFormat, SwapChainFlags);

    ID3D11Texture2D* pBuffer;
    This->GetBuffer(0, __uuidof(ID3D11Texture2D),
        (void**)&pBuffer);
    // Perform error handling here!

    pDevice->CreateRenderTargetView(pBuffer, NULL, &pRenderTargetView);
    // Perform error handling here!
    pBuffer->Release();

    pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

   
    // Set up the viewport.
    D3D11_VIEWPORT vp;
    vp.Width = Width;
    vp.Height = Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1, &vp);
    return hr;
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyscInterval, UINT Flags)
{

    //Helpers::UnHookFunction((PVOID*)(&oPresent), hkPresent);
    //ID3D11RenderTargetView* MainTargetView = nullptr;
    
    static bool isFirst = TRUE;
    if (isFirst)
    {
        isFirst = !isFirst;
        //得到游戏窗体
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)(&pDevice))))
            pDevice->GetImmediateContext(&pContext);
        
        
        ID3D11Texture2D* renderTargetTexture = nullptr;

        //获取缓冲区地址
        if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)(&renderTargetTexture))))
        {
            //pRenderTargetView->GetDevice();
            //创建目标视图
            pDevice->CreateRenderTargetView(renderTargetTexture, NULL, &pRenderTargetView);                    

     /*       float color_black = { 0.0f };
            pContext->ClearRenderTargetView(pRenderTargetView, (float*)&color_black);*/
            //释放缓冲区
            renderTargetTexture->Release();
        }
        

        ImGui_ImplWin32_Init(g_hWnd);
        ImGui_ImplDX11_Init(pDevice, pContext);
        ImGui_ImplDX11_CreateDeviceObjects();


        DWORD ndThreadId = GetWindowThreadProcessId(g_hWnd, NULL);
        //WH_CALLWNDPROC
        if (ndThreadId != 0)  g_HookRet = SetWindowsHookEx(WH_CALLWNDPROC, hkWndProc_, NULL, ndThreadId);
        oWndProc = (WNDPROC)SetWindowLongPtr(g_hWnd, WNDPROC_INDEX, (LONG_PTR)hkWndProc);
        //DebugPrintf("HOOK主线程情况： ndThreadId => %X | g_HookRet => %X | oWndProc => %llX \n", ndThreadId, g_HookRet, oWndProc);


    }

    ImGui_ImplDX11_NewFrame(); //创建dx11着色器 已经以及字体
    ImGui_ImplWin32_NewFrame();//获取当前系统鼠标 键盘 游戏手柄的信息
    ImGui::NewFrame();

    GuiDraw();
    

    ImGui::EndFrame();
    ImGui::SetWindowPos(u8"TestImGui", { 700.f,100.f });
    ImGui::Render();
    pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    HRESULT tmpAdrs = oPresent(pSwapChain, SyscInterval, Flags);
    //Helpers::HookFunction((PVOID*)(&oPresent), hkPresent);
    return tmpAdrs;
}

// 程序入口后的线程开始处
void init_d3d11(void* data)
{
    // 设置游戏的类名或者窗口标题，如果此函数被检测，可以更换其他方式获得窗口句柄。
    do
    {
        //g_hWnd = Helpers::GetCurrentWindowHandle();//游戏功能::取窗口句柄();////获取注入后的程序窗口句柄
        g_hWnd = FindWindowA(global::UwinName, global::UWinText);
        global::logouts("%s - g_hWnd=%d \n",__FUNCTION__, g_hWnd);
    } while (!g_hWnd);

    //交接链
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//设置颜色格式为RGBA
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//缩放比
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//扫描线
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//渲染目标输出
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//允许模式切换
    scd.OutputWindow = g_hWnd;//在目标窗口内部绘制另外一个窗口，本处确认目标窗口句柄
    scd.SampleDesc.Count = 1;//1重采样
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//常用参数
    scd.Windowed = ((GetWindowLongPtr(g_hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE; //是否全屏，随目标窗口变化而定
    scd.BufferDesc.Width = 1920;
    scd.BufferDesc.Height = 1080;
    scd.BufferDesc.RefreshRate.Numerator = 144; //刷新率
    scd.BufferDesc.RefreshRate.Denominator = 1; //分母
    scd.SampleDesc.Quality = 0;   //采样等级

    D3D_FEATURE_LEVEL featrueLevel = D3D_FEATURE_LEVEL_11_0;

    D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
        NULL, &featrueLevel, 1, D3D11_SDK_VERSION,
        &scd, &pSwapChain, &pDevice, NULL, &pContext
    );
    SELF_PTR* pSwapChainVT = NULL;

    pSwapChainVT = (SELF_PTR*)*(SELF_PTR*)pSwapChain;
    oPresent = (tD3D11Present)(pSwapChainVT[8]);
    oResizeBuffers = (ResizeBuffers)(pSwapChainVT[13]);

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& Io = ImGui::GetIO();

    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();
    //LoadMyStype();

    //设置不生成imgui窗口位置存贮
    Io.WantSaveIniSettings = false;
    Io.IniFilename = NULL;

    ImFontConfig f_cfg;
    f_cfg.FontDataOwnedByAtlas = false;

    //字体加载
    const ImFont* font = Io.Fonts->AddFontFromMemoryTTF((void*)baidu_font_data, baidu_font_size, 16.0f, &f_cfg, Io.Fonts->GetGlyphRangesChineseFull());
    //const ImFont* font = Io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\YaHei.Consolas.1.12.ttf", 18.0f, NULL, Io.Fonts->GetGlyphRangesChineseFull());

    Helpers::HookFunction((PVOID*)(&oPresent), hkPresent);
    Helpers::HookFunction((PVOID*)(&oResizeBuffers), hkResizeBuffers);
}

void unLoad()
{    
    //卸载主线程HOOK    
    if (g_HookRet) UnhookWindowsHookEx(g_HookRet);

    //卸载D3D11指针HOOK  
    if (oPresent) Helpers::UnHookFunction((PVOID*)(&oPresent), hkPresent);      
    if (oResizeBuffers) Helpers::UnHookFunction((PVOID*)(&oResizeBuffers), hkResizeBuffers);
}

void GameInit()
{

    //g_hWnd = Helpers::GetCurrentWindowHandle();//游戏功能::取窗口句柄();////获取注入后的程序窗口句柄

    //加载HOOK明文包();
}
// 程序入口
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  reason_call, LPVOID lpReserved)
{
    switch (reason_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        // 优化dll线程
        DisableThreadLibraryCalls(hModule);
        // 开始载入hook线程---干我们的事情
        ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)init_d3d11, NULL, NULL, NULL);

        break;
    } 
    case DLL_THREAD_ATTACH: break;
    case DLL_THREAD_DETACH: break;
    case DLL_PROCESS_DETACH:unLoad();break;
    }
    return TRUE;
}
