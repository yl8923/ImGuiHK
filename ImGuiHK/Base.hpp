#include <Windows.h>



enum Base
{
	UWorld_ = 0x777777,		//UWorldƫ��
	UFName_ = 0x777777,		//UFNameƫ��
	GObject = 0x777777,		//Objectƫ��
	Matrix_ = 0x777777,		// �����ַ�ײ�ƫ��[["torchlight_infinite.exe"+08869920]+0x20]+0x280
	//����ͨ�������룺00 00 00 00 00 00 00 00 00 00 80 3F 00 00 80 3F 00 00 80 3F 00 00 80 3F 00 00 00 00 00 00 00 00 00 00 80 3F 00 00 00 00 00 00 00 00 00 00 80 3F 00 00 80 3F 00 00 00 00
	//[Canvas Transient.DebugCanvasObject]+0x280


	// UE����ƫ��
	Pos01 = 0x130,
	Pos02_x = 0x1D0,
	Pos02_y = 0x1D4,
	Pos02_z = 0x1D8,

};


namespace global
{
	//	���ھ����ȡ����
	static const char* UwinName = "UnrealWindow";	//��������
	static const char* UWinText = NULL;				//���ڱ���

	//	��־���
	inline void logouts(char* pszFormat, ...)
	{
		char szbufFormat[0x1000];
		char szbufFormat_Game[0x1100] = "[AeiyDbg]:";
		va_list argList;
		va_start(argList, pszFormat);//�����б��ʼ��
		vsprintf_s(szbufFormat, pszFormat, argList);
		strcat_s(szbufFormat_Game, szbufFormat);
		OutputDebugStringA(szbufFormat_Game);
		va_end(argList);
	}

	// ���ڴ�
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