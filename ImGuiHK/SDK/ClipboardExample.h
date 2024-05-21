#pragma once
class ClipboardExample
{
public:
    ClipboardExample();
    ~ClipboardExample();

    BOOL SetClipData(char* pstr);
    char* GetClipData();
    char* GetError();
private:
    DWORD errCode;
    HGLOBAL hClip;
};
