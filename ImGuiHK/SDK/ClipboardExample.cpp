#include <Windows.h>
#include "ClipboardExample.h"


ClipboardExample::ClipboardExample()
 {
     errCode=0;
     hClip=NULL;
 }
 
 ClipboardExample::~ClipboardExample()
 {
     if(hClip)
         CloseHandle(hClip);
 }
 
 BOOL ClipboardExample::SetClipData(char *pstr)
 {
     if(OpenClipboard(NULL))
     {
         char *pBuf;
         if(0==EmptyClipboard())
         {
             CloseClipboard();
             return false;
         }
         hClip=GlobalAlloc(GMEM_MOVEABLE,strlen(pstr)+1);
         if(NULL==hClip)
         {
             CloseClipboard();
             return false;
         }
         pBuf=(char*)GlobalLock(hClip);
         if(NULL==pBuf)
         {
             CloseClipboard();
             return false;
         }
         strcpy(pBuf,pstr);
         GlobalUnlock(hClip);
         
         if(NULL==SetClipboardData(CF_TEXT,hClip))
         {
             CloseClipboard();
             return false;
         }
 
         CloseClipboard();
     }
     return true;
 }
 
 char* ClipboardExample::GetClipData()
 {
     char* pstr=0;
     if(OpenClipboard(NULL))
     {
         if(IsClipboardFormatAvailable(CF_TEXT))
         {
             hClip = GetClipboardData(CF_TEXT);
             if(NULL==hClip)
             {
                 CloseClipboard();
                 return NULL;
             }
             pstr = (char*)GlobalLock(hClip);
             GlobalUnlock(hClip);
             CloseClipboard();
         }
     }
     return pstr;
 }
 
 char* ClipboardExample::GetError()
 {
     errCode = GetLastError();
     char pstr[128];
     itoa(errCode,pstr,10);
     return pstr;
 }