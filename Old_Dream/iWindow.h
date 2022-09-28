#pragma once

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "resource.h"

#define Debug

#ifdef Debug
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif //_DEBUG

#define GDIPVER 0x0110
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;


wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(wchar_t* str);
