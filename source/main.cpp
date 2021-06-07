#include <Windows.h>
#include <cstdio>

// https://stackoverflow.com/questions/26005744/how-to-display-pixels-on-screen-directly-from-a-raw-array-of-rgb-values-faster-t
// TODO: draw pixel arrays

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY: {
            ::PostQuitMessage(0);
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hWnd, &ps);
            return 0;
        }
        default: {
            return ::DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }
}

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

    HWND hWnd = ::CreateWindow(wc.lpszClassName,     // lpClassName
                               "Renderer",           // lpWindowName
                               WS_OVERLAPPEDWINDOW,  // dwStyle
                               CW_USEDEFAULT,        // x
                               CW_USEDEFAULT,        // y
                               1200,                 // nWidth
                               800,                  // nHeight
                               NULL,                 // hWndParent
                               NULL,                 // hMenu
                               wc.hInstance,         // hInstance
                               NULL                  // lpParam
    );

    if (!hWnd) {
        printf("Failed to create window");
        return 1;
    }

    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hWnd);

    MSG msg{};
    while (msg.message != WM_QUIT) {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }
    }

    ::DestroyWindow(hWnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}
