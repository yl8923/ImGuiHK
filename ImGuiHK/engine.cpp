
#include "pch.h"
#include "Base.hpp"
#include "engine.h"
#include <vector>
#include "Matrix.h"
#include "Game.h"


#pragma warning(disable:4996)

extern HWND g_hWnd;

UWorld* World;
TUObjectArray* ObjObjects = nullptr;
FNamePool* NamePoolData = nullptr;

UINT64 g_UWORLD_OFFSET	= Base::UWorld_;	//UWorld偏移
UINT64 g_UOBJECT_OFFSET = Base::GObject;	//UObject偏移
UINT64 g_UFNAME_OFFSET	= Base::UFName_;	//UName偏移
UINT64 g_Matrix_Addr	= Base::Matrix_;	//矩阵地址[["torchlight_infinite.exe"+08869920]+20]+280

extern float drawcolor[4];//绘制字体的颜色变量

UINT64 ReadQword(UINT64 lptr) { return *(UINT64*)lptr; }
DWORD ReadDword(UINT64 lptr) { return *(DWORD*)lptr; }
WORD ReadWord(UINT64 lptr) { return *(WORD*)lptr; }

std::string FNameEntry::String()
{
	if (bIsWide) { return std::string(); }
	return { AnsiName, Len };
}

FNameEntry* FNamePool::GetEntry(FNameEntryHandle handle) const
{
	return reinterpret_cast<FNameEntry*>(Blocks[handle.Block] + 2 * static_cast<uint64_t>(handle.Offset));
}

std::string FName::GetName()
{
	auto entry = NamePoolData->GetEntry(Index);
	auto name = entry->String();
	if (Number > 0)
	{
		name += '_' + std::to_string(Number);
	}
	auto pos = name.rfind('/');
	if (pos != std::string::npos)	
		name = name.substr(pos + 1);	
	return name;
}

std::string UObject::GetName()
{
	return NamePrivate.GetName();
}

std::string UObject::GetFullName()
{
	std::string name;
	for (auto outer = OuterPrivate; outer; outer = outer->OuterPrivate) { name = outer->GetName() + "." + name; }
	name = ClassPrivate->GetName() + " " + name + this->GetName();
	return name;
}

bool UObject::IsA(void* cmp)
{
	for (auto super = ClassPrivate; super; super = static_cast<UClass*>(super->SuperStruct)) 
	{ 
		if (super == cmp) return true;  
	}
	return false;
}

void UObject::ProcessEvent(void* fn, void* parms)
{
	auto vtable = *reinterpret_cast<void***>(this);
	reinterpret_cast<void(*)(void*, void*, void*)>(vtable[0x45-1])(this, fn, parms);
}

UObject* FindObjectByIndex(TUObjectArray* ObjObjects, uint32_t id)
{
	if (id >= ObjObjects->NumElements) return nullptr;
	uint64_t chunkIndex = id / 65536;
	if (chunkIndex >= ObjObjects->NumChunks) return nullptr;
	auto chunk = ObjObjects->Objects[chunkIndex];
	if (!chunk) return nullptr;
	uint32_t withinChunkIndex = id % 65536 * 24;
	auto item = *reinterpret_cast<UObject**>(chunk + withinChunkIndex);
	return item;
}

UObject* FindObject(TUObjectArray* ObjObjects, const char* name)
{
	for (auto i = 0u; i < ObjObjects->NumElements; i++)
	{
		auto object = FindObjectByIndex(ObjObjects, i);
		if (object && object->GetFullName() == name) return object; 
	}
	return nullptr;
}

UObject* TUObjectArray::GetObjectPtr(uint32_t id) const
{
	if (id >= NumElements) return nullptr;
	uint64_t chunkIndex = id / 65536;
	if (chunkIndex >= NumChunks) return nullptr;
	auto chunk = Objects[chunkIndex];
	if (!chunk) return nullptr;
	uint32_t withinChunkIndex = id % 65536 * 24;
	auto item = *reinterpret_cast<UObject**>(chunk + withinChunkIndex);
	return item;
}

UObject* TUObjectArray::FindObject(const char* name) const
{
	for (auto i = 0u; i < NumElements; i++)
	{
		auto object = GetObjectPtr(i);
		if (object && object->GetFullName() == name) { return object; }
	}
	return nullptr;
}

// UE4通用ID取Name
const char* GetNameFromFName(int key)
{
	auto ReadVirtual = [](LPVOID src, LPVOID dest, size_t sz) 
		{
			if (IsBadReadPtr(src, sz))
			{
				RtlZeroMemory(dest, sz);
				return;
			}
			RtlCopyMemory(dest, src, sz);
		};

	ULONG64 fNamePool = g_UFNAME_OFFSET + (ULONG64)GetModuleHandleA(NULL);
	DWORD chunkOffset = ((int)(key) >> 16); // Block
	WORD nameOffset = key;
	auto namePoolChunk = ReadQword(fNamePool + ((chunkOffset + 2) * 8));
	auto entryOffset = namePoolChunk + (DWORD)(2 * nameOffset);
	WORD nameLength = ReadWord(entryOffset) >> 6;
	if (nameLength > 256)nameLength = 255;
	static char cBuf[256];
	ReadVirtual((LPVOID)(entryOffset + 2), cBuf, nameLength);
	cBuf[nameLength] = 0;
	return cBuf;
}



std::string ID取名字(int key)
{
	ULONG64 fNamePool = g_UFNAME_OFFSET + (ULONG64)GetModuleHandleA(NULL);
	DWORD chunkOffset = ((int)(key) >> 16); // Block
	WORD nameOffset = key;
	auto namePoolChunk = ReadQword(fNamePool + ((chunkOffset + 2) * 8));
	auto entryOffset = namePoolChunk + (DWORD)(2 * nameOffset);
	WORD nameLength = ReadWord(entryOffset) >> 6;
	if (nameLength > 256)nameLength = 255;
	std::string tmpstr = "";
	tmpstr = (char*)(ReadQword(entryOffset + 2));
	return tmpstr;
}


void EngineEntry(char* tmpNamedd)
{
	using namespace global;

	UINT64 GameBase = (UINT64)GetModuleHandleA(nullptr);//取EXE主模块的句柄
	UINT64 UWorld = *(UINT64*)(GameBase + g_UWORLD_OFFSET );
	
	UINT64 UArray = Read(UWorld, { 0x30,0x98 });
	DWORD UArrNum = *(DWORD*)(Read(UWorld, {0x30}) + 0x98 + 0x8);

	static float pos[3] = {0};
	UINT64 matrixAddrs = Read(*(UINT64*)(GameBase + g_Matrix_Addr), {0x20}) + 0x280; // [["torchlight_infinite.exe" + 08869920]+ 20] + 280

	Matrix mat(matrixAddrs,g_hWnd);

	if (!mat.GetMatrixArr()) return;

	for (size_t i = 0; i < UArrNum; i++)
	{
		UINT64 tmpObj = *(UINT64*)(UArray + i * 0x8);
		if (!tmpObj) continue;
		
		UINT64 tmpPosObj = *(UINT64*)(tmpObj + Base::Pos01);
		if (!tmpPosObj ) continue;
		
		pos[0] = *(float*)(tmpPosObj + Base::Pos02_x);
		pos[1] = *(float*)(tmpPosObj + Base::Pos02_y);
		pos[2] = *(float*)(tmpPosObj + Base::Pos02_z);

		int x = 0, y = 0, w = 0, h = 0;
		if (mat.GetCoreInfo(pos , x , y , w , h ))
		{
			int ID = *(int*)(tmpObj + 0x18);

			std::string tmpName = GetNameFromFName(ID);

			ImVec2 tmpPos( x , y );

			if (tmpName.find(tmpNamedd) != std::string::npos)
				ImGui::GetForegroundDrawList()->AddText(
					tmpPos, 
					ImGui::ColorConvertFloat4ToU32(ImVec4(drawcolor[0], drawcolor[1], drawcolor[2], drawcolor[3])),
					tmpName.c_str());
		}	
	}

}