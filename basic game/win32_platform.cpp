#include<windows.h>

LRESULT CALLBACK window_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

ATOM RegisterClass(const WNDCLASSA* lpWndClass );

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// create window class 
	WNDCLASS Window_class = {};
	Window_class.style = CS_HREDRAW | CS_VREDRAW;
	Window_class.lpszClassName = L"Game Window Class";
	Window_class.lpfnWndProc = window_callback;


	// register class
	RegisterClass(&Window_class);


	// create windows

	CreateWindow(
		Window_class.lpszClassName, L"My First game!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0
	);



}