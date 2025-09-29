#include "utils.cpp"
#include <windows.h>

// Global variables to keep track of the running state of the program
global_variable bool running = true;

// Structure to store rendering state and pixel buffer information
struct Render_State
{
    int width, height;
    void *memory;
    BITMAPINFO bit_map_info;
};

global_variable Render_State rander_state;

// Include essential files for platform-specific operations and rendering
#include "platform_common.cpp"
#include "renderer.cpp"
#include "game_file.cpp"

// Window callback function to handle various window messages
LRESULT CALLBACK window_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (Msg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
    {
        // When the window is closed or destroyed, stop running the program
        running = false;
    }
    break;

    case WM_SIZE:
    {
        // Handle window resizing by updating buffer dimensions
        RECT rect;
        GetClientRect(hWnd, &rect);
        rander_state.width = rect.right - rect.left;
        rander_state.height = rect.bottom - rect.top;

        // Allocate memory for the buffer
        int buffersize = rander_state.width * rander_state.height * sizeof(unsigned int);

        // If there is existing buffer memory, free it before allocating new memory
        if (rander_state.memory)
            VirtualFree(rander_state.memory, 0, MEM_RELEASE);

        // Allocate new buffer memory
        rander_state.memory = VirtualAlloc(0, buffersize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        // Configure bitmap settings for rendering
        rander_state.bit_map_info.bmiHeader.biSize = sizeof(rander_state.bit_map_info.bmiHeader);
        rander_state.bit_map_info.bmiHeader.biWidth = rander_state.width;
        rander_state.bit_map_info.bmiHeader.biHeight = rander_state.height;
        rander_state.bit_map_info.bmiHeader.biPlanes = 1;
        rander_state.bit_map_info.bmiHeader.biBitCount = 32;
        rander_state.bit_map_info.bmiHeader.biCompression = BI_RGB;
    }
    break;

    default:
    {
        // For any other messages, use the default window procedure
        result = DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    }

    return result;
}

// Entry point function for the Windows application
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // Create and configure a window class
    WNDCLASS Window_class = {};
    Window_class.style = CS_HREDRAW | CS_VREDRAW;      // Redraw on horizontal/vertical resizing
    Window_class.lpszClassName = L"Game Window Class"; // Window class name
    Window_class.lpfnWndProc = window_callback;        // Pointer to window callback function

    // Register the window class
    RegisterClass(&Window_class);

    // Create the window with defined attributes
    HWND window = CreateWindow(
        Window_class.lpszClassName, L"My First Game!",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style with visibility
        CW_USEDEFAULT, CW_USEDEFAULT,     // Default position
        1280, 720,                        // Window dimensions
        0, 0,                             // Parent and menu handles (none)
        hInstance,                        // Application instance handle
        0                                 // Additional parameters (none)
    );

    // Main game loop: Continuously processes messages and keeps the program running
    Input input = {};

    float delta_time = 0.016666f;

    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);

    float performance_frequency;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)(perf.QuadPart);
    }

    while (running)
    {
        MSG message;
        HDC hdc = GetDC(window); // Retrieve the device context for rendering

        // Reset button change states before processing input
        for (int i = 0; i < BUTTON_COUNT; i++)
        {
            input.buttons[i].changed = false;
        }

        // Process messages for the window
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
        {
            switch (message.message)
            {
            case WM_KEYUP:
            case WM_KEYDOWN:
            {
                u32 vk_code = (u32)message.wParam;
                bool is_down = ((message.lParam & (1 << 31)) == 0);

#define process_button(b, vk)\
case vk:\
{\
input.buttons[b].changed = is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
}\
break;\
                // Handle specific key press inputs
                switch (vk_code)
                {
                    process_button(BUTTON_UP, VK_UP);
                    process_button(BUTTON_DOWN, VK_DOWN);
                    process_button(BUTTON_LEFT, VK_LEFT);
                    process_button(BUTTON_RIGHT, VK_RIGHT);
                    process_button(BUTTON_W, 'W');
                    process_button(BUTTON_S, 'S');
                    process_button(BUTTON_ENTER, VK_RETURN);
                }
            }
            break;
            default:
            {
                // Translate and dispatch other window messages
                TranslateMessage(&message);
                DispatchMessageW(&message);
            }
            }
        }

        // Simulate and update game logic
        simulate_game(&input, delta_time);

        // Render the updated game screen
        StretchDIBits(hdc, 0, 0, rander_state.width, rander_state.height,
                      0, 0, rander_state.width, rander_state.height,
                      rander_state.memory, &rander_state.bit_map_info,
                      DIB_RGB_COLORS, SRCCOPY);

        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);

        delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
        frame_begin_time = frame_end_time;
    }

    return 0;
}