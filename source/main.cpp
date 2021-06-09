#include <Windows.h>
#include <cstdio>
#include "rasterizer.h"

// https://stackoverflow.com/questions/26005744/how-to-display-pixels-on-screen-directly-from-a-raw-array-of-rgb-values-faster-t
// TODO: draw pixel arrays

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        default:
            return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

constexpr int WIDTH = 1200;
constexpr int HEIGHT = 800;

int main() {
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),     // cbSize
        CS_CLASSDC,             // sylte
        WndProc,                // plfnWndProc
        0,                      // cbClsExtra
        0,                      // cbWndExxtra
        GetModuleHandle(NULL),  // hInstance
        NULL,                   // hIcon
        NULL,                   // hCursor
        NULL,                   // hbrBackground
        NULL,                   // lpszMenuName
        "MainWindow",           // lpszClassName
        NULL,                   // hIconSm
    };

    ::RegisterClassEx(&wc);

    HWND hWnd = ::CreateWindow(wc.lpszClassName,            // lpClassName
                               "Renderer",                  // lpWindowName
                               WS_OVERLAPPED | WS_SYSMENU,  // dwStyle
                               CW_USEDEFAULT,               // x
                               CW_USEDEFAULT,               // y
                               WIDTH,                       // nWidth
                               HEIGHT,                      // nHeight
                               NULL,                        // hWndParent
                               NULL,                        // hMenu
                               wc.hInstance,                // hInstance
                               NULL                         // lpParam
    );

    if (!hWnd) {
        printf("Failed to create window");
        return 1;
    }

    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hWnd);

    // TODO: allow resizing
    HDC hDC = GetDC(hWnd);
    BITMAP bitmap{};
    HGDIOBJ hBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    rs::initialize();
    rs::createCanvas(bitmap.bmWidth, bitmap.bmHeight);

    MSG msg{};
    while (msg.message != WM_QUIT) {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // static int c = 0;
        // rs::clearColor(c, c, c, 255);
        rs::clear(rs::ColorBufferBit);
        rs::drawTriangle(glm::vec2(-.5f, -.5f), glm::vec2(.0f, .5f), glm::vec2(.5f, -.5f));

        // Creating temp bitmap
        HBITMAP map = CreateBitmap(bitmap.bmWidth,       // width. 512 in my case
                                   bitmap.bmHeight,      // height
                                   1,                    // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
                                   32,                   // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
                                   rs::getCanvasRaw());  // pointer to array
        // Temp HDC to copy picture
        HDC src = CreateCompatibleDC(hDC);  // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
        SelectObject(src, map);             // Inserting picture into our temp HDC
        // Copy image from temp HDC to window
        BitBlt(hDC,              // Destination
               0,                // x and
               0,                // y - upper-left corner of place, where we'd like to copy
               bitmap.bmWidth,   // width of the region
               bitmap.bmHeight,  // height
               src,              // source
               0,                // x and
               0,                // y of upper left corner  of part of the source, from where we'd like to copy
               SRCCOPY);         // Defined DWORD to juct copy pixels. Watch more on msdn;

        DeleteObject(map);
        DeleteDC(src);  // Deleting temp HDC
    }

    rs::finalize();

    ::ReleaseDC(hWnd, hDC);
    ::DestroyWindow(hWnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}
