#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <ShlObj.h>
#include <tchar.h>
#include <stdlib.h>
#include "fileReplacer.h"

int main() {
    char* path = "C:\\Users\\chids\\Documents";
    printf("%s\n", path);

    TCHAR *modPath = getPath(L"Select Mods Folder");
    char strModPath[MAX_PATH];

    wcstombs(strModPath, modPath, wcslen(modPath) + 1);
    printf("%s\n", strModPath);
    listFiles(strModPath);

    TCHAR* srcPath = getPath(L"Select Decompiled Game Folder");
    char strSrcPath[MAX_PATH];

    wcstombs(strSrcPath, srcPath, wcslen(srcPath) + 1);
    printf("%s\n", strSrcPath);
    listFiles(strModPath);

    return 0;
}