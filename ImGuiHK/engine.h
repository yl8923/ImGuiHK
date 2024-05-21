#pragma once
#include <cstdint>
#include <windows.h>
#include <string>
#include <locale>
#undef DrawText

struct FVector {
	float X, Y, Z;

	FVector() : X(0.f), Y(0.f), Z(0.f) {}
	FVector(float X, float Y, float Z) : X(X), Y(Y), Z(Z) {}
	FVector(float InF) : X(InF), Y(InF), Z(InF) { }
	float Size() const { return sqrtf(X * X + Y * Y + Z * Z); }
	float DistTo(const FVector& V) const { return (*this - V).Size(); }
	FVector operator-(const FVector& other) const { return FVector(X - other.X, Y - other.Y, Z - other.Z); }
};

struct FVector2D {
	float X, Y;
};

struct FRotator {
	float Pitch, Yaw, Roll;
};

struct FLinearColor {
	float R, G, B, A;
	FLinearColor() : R(0.f), G(0.f), B(0.f), A(0.f) {};
	FLinearColor(float R, float G, float B, float A) : R(R), G(G), B(B), A(A) {};
};

template<typename T>
struct TArray {
	T* Data;
	int Count;
	int Size;
};

struct FString : private TArray<wchar_t>
{
	FString() = default;

	explicit FString(const wchar_t* other)
	{
		Size = Count = *other ? std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

struct FNameEntryHandle {
	uint32_t Block = 0;
	uint32_t Offset = 0;

	FNameEntryHandle(uint32_t block, uint32_t offset) : Block(block), Offset(offset) {};
	FNameEntryHandle(uint32_t id) : Block(id >> 16), Offset(id & 65535) {};
	operator uint32_t() const { return (Block << 16 | Offset); }
};

struct FNameEntry {
	uint16_t bIsWide : 1;
	uint16_t LowercaseProbeHash : 5;
	uint16_t Len : 10;
	union
	{
		char AnsiName[1024];
		wchar_t	WideName[1024];
	};

	std::string String();
};

struct FNamePool 
{
	byte Lock[8];
	uint32_t CurrentBlock;
	uint32_t CurrentByteCursor;
	byte* Blocks[8192];

	FNameEntry* GetEntry(FNameEntryHandle handle) const;
};

struct FName {
	uint32_t Index;
	uint32_t Number;

	std::string GetName();
};

struct UObject {
	void** VFTable;
	uint32_t ObjectFlags;
	uint32_t InternalIndex;
	struct UClass* ClassPrivate;
	FName NamePrivate;
	UObject* OuterPrivate;

	std::string GetName();
	std::string GetFullName();
	bool IsA(void* cmp);
	void ProcessEvent(void* fn, void* parms);
};


// Class CoreUObject.Field
// Size: 0x30 (Inherited: 0x28)
struct UField : UObject {
	char UnknownData_28[0x8]; // 0x28(0x8)
};

// Class CoreUObject.Struct
// Size: 0xb0 (Inherited: 0x30)
struct UStruct : UField {
	char UnknownData_30[0x10]; // 0x30(0x10)
	UStruct* SuperStruct; // 0x40(0x8)
	char UnknownData_48[0x68]; // 0x48(0x80)
};

// Class CoreUObject.Class
// Size: 0x230 (Inherited: 0xb0)
struct UClass : UStruct {
	char pad_b0[0x68];
	UObject* ClassDefaultObject; // 0x118
	char pad_120[0x110];
	static UClass* StaticClass();
};

// Class CoreUObject.Property
// 0x040 (FullSize[0x070] - InheritedSize[0x030])
class UProperty : public UField
{
public:
	unsigned char                                      UnknownData_WINC[0x40];                                    // 0x030(0x040) MISSED OFFSET (PADDING)
};

// Class CoreUObject.Function
// 0x030 (FullSize[0x0C8] - InheritedSize[0x098])
class UFunction : public UStruct
{
public:
	int32_t FunctionFlags; // 0x098(0x4) NOT AUTO-GENERATED PROPERTY
	int16_t RepOffset; // 0x09C(0x2) NOT AUTO-GENERATED PROPERTY
	int8_t NumParms; // 0x09E(0x1) NOT AUTO-GENERATED PROPERTY
	unsigned char pad_SQAJA5HJJT[0x1]; // 0x09F(0x1) NOT AUTO-GENERATED PROPERTY
	int16_t ParmsSize; // 0x0A0(0x2) NOT AUTO-GENERATED PROPERTY
	uint16_t ReturnValueOffset; // 0x0A2(0x2) NOT AUTO-GENERATED PROPERTY
	uint16_t RPCId; // 0x0A4(0x2) NOT AUTO-GENERATED PROPERTY
	uint16_t RPCResponseId; // 0x0A6(0x2) NOT AUTO-GENERATED PROPERTY
	class UProperty* FirstPropertyToInit; // 0x0A8(0x8) NOT AUTO-GENERATED PROPERTY
	class UFunction* EventGraphFunction; // 0x0B0(0x8) NOT AUTO-GENERATED PROPERTY
	int32_t EventGraphCallOffset;                                      // 0x0B8(0x4) NOT AUTO-GENERATED PROPERTY
	unsigned char pad_K3Z6JGYGNY[0x4];                                      // 0x0BC(0x4) NOT AUTO-GENERATED PROPERTY
	void* Func; // 0x0C0(0x8) NOT AUTO-GENERATED PROPERTY

};

struct TUObjectArray {
	byte** Objects;
	byte* PreAllocatedObjects;
	uint32_t MaxElements;
	uint32_t NumElements;
	uint32_t MaxChunks;
	uint32_t NumChunks;

	UObject* GetObjectPtr(uint32_t id) const;
	UObject* FindObject(const char* name) const;
};

// Class Engine.Level
// Size: 0x178 (Inherited: 0x28)
struct ULevel : UObject {
	char pad_0[0x30]; //0x
	wchar_t* LevelLogo; //0x058
	char pad_0060[56]; //0x060
	//AActor** Actor; //0x098
	int32_t ActorCount; //0x0A0
	int32_t MaxActor; //0x0A4
	//AActor** Actor2; //0x0A8
	int32_t UnknownCount; //0x0B0
	int32_t MaxUnkCount; //0x0B4
};
// Class Engine.World
// Size: 0x178 (Inherited: 0x28)
struct UWorld : UObject {
	char pad_08[0x8]; //0x28(0x8)
	struct ULevel* PersistentLevel; //0x30(0x8)
	char pad_f8[0xF8]; //0x38(0xF8)
	struct AGameStateBase* GameState; // 0x130(0x08)
	char pad_0038[0x38]; //0x138(0x0x38)
	struct UGameInstance* OwningGameInstance; //0x170(0x8)
};

// Class Engine.ScriptViewportClient
// Size: 0x38 (Inherited: 0x28)
struct UScriptViewportClient : UObject {
	char UnknownData_28[0x10]; // 0x28(0x10)
};

// Class Engine.GameViewportClient
// Size: 0x330 (Inherited: 0x38)
struct UGameViewportClient : UScriptViewportClient {
	char UnknownData_38[0x40]; // 0x38(0x8)
	char UnknownData_78[0x8]; 
	//struct UWorld* World; // 0x78(0x8)
	struct UGameInstance* GameInstance; // 0x80(0x8)
	char UnknownData_88[0x2A8]; // 0x88(0x2A8)
};






// Class Engine.Engine
// Size: 0x7D0 (Inherited: 0x28)
struct UEngine : UObject {
	char pad_b8[0x7A0]; // 0x28(0x7A0)
	struct UGameViewportClient* GameViewport; // 0x7C8(0x8)
};

struct FTableRowBase {
	char pad_0[0x8];
}; // Size: 0x8

struct FWindow_TableEntity : FTableRowBase {
	int32_t ID; // 0x8
	char pad_c[0x4];
	FString resource_path; // 0x10
	FString mobile_resource_path; // 0x20
	bool isinstancewindow; // 0x30
	bool duplicatecreation; // 0x31
	bool dontdestroy; // 0x32
	char pad_33[0x1];
	float LifeTime; // 0x34
	bool isfullscreen; // 0x38
	bool iscamerapossess; // 0x39
	bool iscameralookatemptyspace; // 0x3a
	bool isuionly; // 0x3b
	bool stackevent; // 0x3c
	char pad_3d[0x3];
	int32_t eventpriority; // 0x40
	int32_t z_order; // 0x44
	bool showallpage_layer; // 0x48
	char pad_49[0x7];
	FString init_parameter; // 0x50
	bool open_questcomplete; // 0x60
	bool forcedclosebykeybinding; // 0x61
	bool different_ui_open_keybinding; // 0x62
	bool close_ui_keybinding; // 0x63
	char pad_64[0x4];
}; // Size: 0x68

struct FWeight_TableEntity : FTableRowBase {
	int32_t ID; // 0x8
	int32_t weight_des_id; // 0xc
	int32_t min_value; // 0x10
	int32_t max_value; // 0x14
	TArray<int32_t> ability_group_id; // 0x18
	FString icon_path; // 0x28
}; // Size: 0x38




extern UEngine** Engine;
extern UWorld* World;
extern FNamePool* NamePoolData;
extern TUObjectArray* ObjObjects;

extern UINT64 g_UWORLD_OFFSET;
extern UINT64 g_UOBJECT_OFFSET;
extern UINT64 g_UFNAME_OFFSET;




UObject* FindObjectByIndex(TUObjectArray* ObjObjects, uint32_t id);
UObject* FindObject(TUObjectArray* ObjObjects, const char* name);
UINT64 ReadQword(UINT64 lptr); 
WORD ReadWord(UINT64 lptr);
std::string IDÈ¡Ãû×Ö(int key);
const char* GetNameFromFName(int key);
void EngineEntry(char* tmpNamedd);