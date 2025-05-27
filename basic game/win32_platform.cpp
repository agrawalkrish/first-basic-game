#include<windows.h>


bool running = true;
LRESULT CALLBACK window_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (Msg) {
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		}break;

		default: {
			result = DefWindowProc(hWnd, Msg, wParam, lParam);
		}


	}
	return result;
}

//ATOM RegisterClass(const WNDCLASSA* lpWndClass );

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// create window class 
	WNDCLASS Window_class = {};
	Window_class.style = CS_HREDRAW | CS_VREDRAW;
	Window_class.lpszClassName = L"Game Window Class";
	Window_class.lpfnWndProc = window_callback;


	// register class
	RegisterClass(&Window_class);


	// create windows

	HWND window= CreateWindow(
		Window_class.lpszClassName, L"My First game!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0
	);

	while (running) {
		// input 
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}

		//simulate
		

		//running


	}

}