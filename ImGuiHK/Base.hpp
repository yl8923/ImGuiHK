#include <Windows.h>



enum Base
{
	UWorld_ = 0x777777,		//UWorld偏移
	UFName_ = 0x777777,		//UFName偏移
	GObject = 0x777777,		//Object偏移
	Matrix_ = 0x777777,		// 矩阵地址首层偏移[["torchlight_infinite.exe"+08869920]+0x20]+0x280
	//矩阵通用特征码：00 00 00 00 00 00 00 00 00 00 80 3F 00 00 80 3F 00 00 80 3F 00 00 80 3F 00 00 00 00 00 00 00 00 00 00 80 3F 00 00 00 00 00 00 00 00 00 00 80 3F 00 00 80 3F 00 00 00 00
	//[Canvas Transient.DebugCanvasObject]+0x280


	// UE坐标偏移
	Pos01 = 0x130,
	Pos02_x = 0x1D0,
	Pos02_y = 0x1D4,
	Pos02_z = 0x1D8,

};


namespace global
{
	//	窗口句柄获取参数
	static const char* UwinName = "UnrealWindow";	//窗口类名
	static const char* UWinText = NULL;				//窗口标题

	//	日志输出
	inline void logouts(char* pszFormat, ...)
	{
		char szbufFormat[0x1000];
		char szbufFormat_Game[0x1100] = "[AeiyDbg]:";
		va_list argList;
		va_start(argList, pszFormat);//参数列表初始化
		vsprintf_s(szbufFormat, pszFormat, argList);
		strcat_s(szbufFormat_Game, szbufFormat);
		OutputDebugStringA(szbufFormat_Game);
		va_end(argList);
	}

	// 读内存
	template<class T>
	T Read(T addr, std::vector<int> offset)
	{
		T tmpAddr = addr;
		if (IsBadReadPtr((void*)tmpAddr, sizeof(T))) return NULL;

		for (auto it : offset)
			tmpAddr = *(T*)(tmpAddr + it);

		return tmpAddr;
	}

}