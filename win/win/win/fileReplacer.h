#ifndef FILEREPLACER_H
#define FILEREPLACER_H

#include <windows.h>
#include <shlobj.h>
#include <tchar.h>

void listFiles(const char* path);
TCHAR* getPath(TCHAR* msg);
char* TCHARToChar(TCHAR* tcharString);

#endif 
