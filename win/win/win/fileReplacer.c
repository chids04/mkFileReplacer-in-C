#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <shlobj.h>
#include <tchar.h>

#include "fileReplacer.h"

void listFiles(const char* path) {
    char fullPath[MAX_PATH];
    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    strcpy(fullPath, path);
    strcat(fullPath, "\\*.*");

    hFind = FindFirstFileA(fullPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error: %s not found\n", path);
        return;
    }

    do {
        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            printf("%s\\%s\n", path, findData.cFileName);
        }
        else if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
            char subPath[MAX_PATH];
            sprintf(subPath, "%s\\%s", path, findData.cFileName);
            listFiles(subPath);
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);

}
TCHAR* getPath(TCHAR* msg) {
    // Create a buffer to store the selected folder path
    TCHAR folderPath[MAX_PATH];

    // Initialize the BROWSEINFO structure
    BROWSEINFO browseInfo = { 0 };
    browseInfo.lpszTitle = msg;  // Dialog title
    browseInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

    // Show the folder dialog
    LPITEMIDLIST itemIdList = SHBrowseForFolder(&browseInfo);

    // Get the selected folder path
    if (itemIdList != NULL)
    {
        SHGetPathFromIDList(itemIdList, folderPath);
        _tprintf(_T("Selected folder path: %s\n"), folderPath);

        // Clean up
        CoTaskMemFree(itemIdList);
        return folderPath;
    }

    return NULL;
}

char* TCHARToChar(TCHAR* tcharString)
{
    int length = WideCharToMultiByte(CP_UTF8, 0, tcharString, -1, NULL, 0, NULL, NULL);
    char* result = (char*)malloc(length);
    WideCharToMultiByte(CP_UTF8, 0, tcharString, -1, result, length, NULL, NULL);
    return result;
}