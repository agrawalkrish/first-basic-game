#include <windows.h>

// Global variables to keep track of the running state of the program
bool running = true;

// Buffer variables for storing pixel data
void* buffermemory;
int bufferwidth;
int bufferheight;
BITMAPINFO bufferbitmapinfo;

// Window callback function to handle window messages
LRESULT CALLBACK window_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (Msg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        // When the window is closed or destroyed, stop running the program
        running = false;
    } break;

    case WM_SIZE: {
        // Handle window resizing by updating buffer dimensions
        RECT rect;
        GetClientRect(hWnd, &rect);
        bufferwidth = rect.right - rect.left;
        bufferheight = rect.bottom - rect.top;

        // Allocate memory for the buffer
        int buffersize = bufferwidth * bufferheight * sizeof(unsigned int);

        // If there is existing buffer memory, free it before allocating new memory
        if (buffermemory) VirtualFree(buffermemory, 0, MEM_RELEASE);

        // Allocate new buffer memory
        buffermemory = VirtualAlloc(0, buffersize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        bufferbitmapinfo.bmiHeader.biSize = sizeof(bufferbitmapinfo.bmiHeader);
        bufferbitmapinfo.bmiHeader.biWidth = bufferwidth;
        bufferbitmapinfo.bmiHeader.biHeight = bufferheight;
        bufferbitmapinfo.bmiHeader.biPlanes = 1;
        bufferbitmapinfo.bmiHeader.biBitCount = 32;
        bufferbitmapinfo.bmiHeader.biCompression = BI_RGB;

    } break;

    default: {
        // For any other messages, use the default window procedure
        result = DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    }

    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Create and configure a window class
    WNDCLASS Window_class = {};
    Window_class.style = CS_HREDRAW | CS_VREDRAW; // Redraw on horizontal/vertical resizing
    Window_class.lpszClassName = L"Game Window Class"; // Window class name
    Window_class.lpfnWndProc = window_callback; // Pointer to window callback function

    // Register the window class
    RegisterClass(&Window_class);

    // Create the window with defined attributes
    HWND window = CreateWindow(
        Window_class.lpszClassName, L"My First Game!",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style with visibility
        CW_USEDEFAULT, CW_USEDEFAULT, // Default position
        1280, 720, // Window dimensions
        0, 0, // Parent and menu handles (none)
        hInstance, // Application instance handle
        0 // Additional parameters (none)
    );

    // Main game loop: Continually processes messages and keeps program running
    while (running) {
        MSG message;
        HDC hdc = GetDC(window);
        // Process any messages for the window
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }

        // Simulate and update game logic here


        unsigned int* pixel = (unsigned int*)buffermemory;
        for (int y = 0; y < bufferheight; y++) {
            for (int x = 0; x < bufferwidth; x++){
                *pixel++ = x*y;
            }
        }

        // Running loop continues until the window is closed
        StretchDIBits(hdc, 0, 0, bufferwidth, bufferheight, 0, 0, bufferwidth, bufferheight, buffermemory, &bufferbitmapinfo, DIB_RGB_COLORS, SRCCOPY);

    }

    return 0;
}