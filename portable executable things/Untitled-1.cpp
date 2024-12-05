#include <iostream>
#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

using namespace std;

typedef void (*ShowImageFunc)(const wchar_t*);

bool FileExists(const wchar_t* filePath) {
    DWORD fileAttributes = GetFileAttributesW(filePath);
    return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

int main() {
    cout << "YO YO WHAT SUP" << endl;

    wchar_t currentDir[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, currentDir);
    wcout << L"Current directory: " << currentDir << endl;

    const wchar_t* imagePath = L".\\23.bmp";
    if (!FileExists(imagePath)) {
        wcerr << L"Error: File not found! Path: " << imagePath << endl;
        MessageBoxW(NULL, L"File not found!", L"ERROR 4", MB_ICONERROR | MB_OK);
        return 1;
    }

    HINSTANCE hDll = LoadLibrary(TEXT("yo.dll"));
    if (!hDll) {
        DWORD error = GetLastError(); 
        wcerr << L"Error loading DLL! Error code: " << error << endl;
        MessageBoxW(NULL, L"Failed to load DLL!", L"ERROR 5", MB_ICONERROR | MB_OK);
        return 1;
    }

    ShowImageFunc ShowImage = (ShowImageFunc)GetProcAddress(hDll, "ShowImage");
    if (!ShowImage) {
        DWORD error = GetLastError(); 
        wcerr << L"Error retrieving ShowImage from DLL! Error code: " << error << endl;
        MessageBoxW(NULL, L"Failed to find ShowImage in DLL!", L"ERROR 6", MB_ICONERROR | MB_OK);
        FreeLibrary(hDll);
        return 1;
    }

    
    ShowImage(imagePath);

    FreeLibrary(hDll);

    cout << "Yo yo yo! Representing the ABQ! 148 Three to the three to the six to the nine! Whaddup biyatch?! Leave it at the tone!." << endl;

    
    cout << "Press Enter to exit the program..." << endl;
    while (true) {
        if (cin.get() == '\n') break;
    }

    return 0;
}
