#include "pch.h"
#include "Matrix.h"
#include "engine.h"
#include "Game.h"
#include "imgui_sugar.hpp"
#include "SDK/ClipboardExample.h"

extern HWND g_hWnd;

static bool DrawOpen = false;
static 	ImVec4 tmpColor;
char g_tmpStr[500] = {0};
float drawcolor[4] = { 
	ImColor(137, 217, 97).Value.x ,
	ImColor(137, 217, 97).Value.y,
	ImColor(137, 217, 97).Value.z,
	ImColor(137, 217, 97).Value.w
};


std::string str2u8(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string ret = pBuf;
	delete[]pwBuf;
	delete[]pBuf;

	return ret;
}

std::string u82str(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string ret = pBuf;
	delete[]pBuf;
	delete[]pwBuf;
	return ret;
}


void GuiDraw()
{
	if (!g_hWnd) return;

	if (GetAsyncKeyState(VK_HOME) != 0) //����Home���� ���¼�����
	{
		DrawOpen =!DrawOpen;	//�򿪹رջ��Ƶı�������ȡ��
		Sleep(150);
	}

	// ��������
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	// ���ô�С
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

	ImGui::Begin("UE");
	ImGui::Checkbox(u8"���ƶ���[Home]", &DrawOpen);
	ImGui::SameLine();
	ImGui::ColorEdit4("##����������ɫ", drawcolor, ImGuiColorEditFlags_NoInputs);

	if (ImGui::Button("����"))
	{
		ClipboardExample clip;
		clip.SetClipData((char*)u82str(g_tmpStr).c_str());
	}
	ImGui::SameLine();
	if (ImGui::Button("ճ��"))
	{
		ClipboardExample clip;
		strcpy(g_tmpStr, str2u8(clip.GetClipData()).c_str());
	}
	ImGui::SameLine();
	ImGui::InputText(u8"�����ִ�", g_tmpStr, sizeof(g_tmpStr));



	ImGui::End();

	if (DrawOpen) EngineEntry(g_tmpStr);
	
	

}