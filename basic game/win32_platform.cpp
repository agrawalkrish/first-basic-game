#include "utils.cpp"
#include <windows.h>

// Global variables to keep track of the running state of the program
global_variable bool running = true;

// Buffer variables for storing pixel data

struct Render_State {
     int width, height;
    void* memory;
     BITMAPINFO bit_map_info;
};

global_variable Render_State rander_state;




#include "platform_common.cpp"
#include "renderer.cpp"
#include"game_file.cpp"




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
        rander_state.width = rect.right - rect.left;
        rander_state.height = rect.bottom - rect.top;

        // Allocate memory for the buffer
        int buffersize = rander_state.width * rander_state.height * sizeof(unsigned int);

        // If there is existing buffer memory, free it before allocating new memory
        if (rander_state.memory) VirtualFree(rander_state.memory, 0, MEM_RELEASE);

        // Allocate new buffer memory
        rander_state.memory = VirtualAlloc(0, buffersize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        rander_state.bit_map_info.bmiHeader.biSize = sizeof(rander_state.bit_map_info.bmiHeader);
        rander_state.bit_map_info.bmiHeader.biWidth = rander_state.width;
        rander_state.bit_map_info.bmiHeader.biHeight = rander_state.height;
        rander_state.bit_map_info.bmiHeader.biPlanes = 1;
        rander_state.bit_map_info.bmiHeader.biBitCount = 32;
        rander_state.bit_map_info.bmiHeader.biCompression = BI_RGB;

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


    Input input = {};

        
    while (running) {
        MSG message;
        HDC hdc = GetDC(window);
        // Process any messages for the window

        for (int i=0; i<BUTTON_COUNT; i++){
            input.buttons[i].changed = false;
        }

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            switch (message.message) {
            case WM_KEYUP:
            case WM_KEYDOWN: {
                u32 vk_code = (u32)message.wParam;
                bool is_down = ((message.lParam & (1 << 31)) == 0);

                switch (vk_code) {
                case VK_UP: {
                    input.buttons[BUTTON_UP].is_down = is_down;
                    input.buttons[BUTTON_UP].changed = true;
                    
                }
                }
            }break;
            default: {
                TranslateMessage(&message);
                DispatchMessageW(&message);
            }

            }
            
        }

        // Simulate and update game logic here
        simulate_game(input);


        // Running loop continues until the window is closed
        StretchDIBits(hdc, 0, 0, rander_state.width, rander_state.height, 0, 0, rander_state.width, rander_state.height, rander_state.memory, &rander_state.bit_map_info, DIB_RGB_COLORS, SRCCOPY);

    }

    return 0;
}