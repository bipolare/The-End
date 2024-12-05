#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;
using namespace std;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Bitmap* img = (Bitmap*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (img) {
                Graphics g(hdc);
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                int windowWidth = clientRect.right - clientRect.left;
                int windowHeight = clientRect.bottom - clientRect.top;
                g.DrawImage(img, 0, 0, windowWidth, windowHeight);
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            if (img) {
                delete img;
            }
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

extern "C" __declspec(dllexport) void ShowImage(const wchar_t* imagePath) {

    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    Status status = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    if (status != Ok) {
        wchar_t errorMsg[256];
        swprintf_s(errorMsg, 256, L"Failed to initialize GDI+! Status: %d", status);
        MessageBoxW(NULL, errorMsg, L"ERROR 5", MB_ICONERROR | MB_OK);
        return;
    }

    Bitmap* img = new Bitmap(imagePath);
    if (img->GetLastStatus() != Ok) {
        wchar_t errorMsg[256];
        swprintf_s(errorMsg, 256, L"Failed to load image: %d", img->GetLastStatus());
        MessageBoxW(NULL, errorMsg, L"ERROR 1", MB_ICONERROR | MB_OK);
        delete img;
        GdiplusShutdown(gdiplusToken);
        return;
    }

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"ImageWindow";

    if (!RegisterClassW(&wc)) {
        DWORD error = GetLastError();
        wchar_t errorMsg[256];
        swprintf_s(errorMsg, 256, L"Error registering class: %lu", error);
        MessageBoxW(NULL, errorMsg, L"ERROR 2", MB_ICONERROR | MB_OK);
        delete img;
        GdiplusShutdown(gdiplusToken);
        return;
    }

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int windowWidth = 400;
    int windowHeight = 300;

    int windowX = screenWidth - windowWidth;
    int windowY = (screenHeight - windowHeight) / 2; 


    HWND hwnd = CreateWindowExW(
        0,
        L"ImageWindow",
        L"Image Viewer",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        windowX, windowY, 
        windowWidth, windowHeight, 
        NULL, NULL, GetModuleHandle(NULL), NULL
    );

    if (!hwnd) {
        DWORD error = GetLastError();
        wchar_t errorMsg[256];
        swprintf_s(errorMsg, 256, L"Error creating window: %lu", error);
        MessageBoxW(NULL, errorMsg, L"ERROR 3", MB_ICONERROR | MB_OK);
        delete img;
        GdiplusShutdown(gdiplusToken);
        return;
    }

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)img);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
}
