/*
Pacman - A recreation made by Charlie Anderson
5/10/21

Here is a change
*/



#include <windows.h>

bool running = true;
void* bufferMemory;
int bufferWidth;
int bufferHeight;
BITMAPINFO bufferBitmapInfo;

LRESULT CALLBACK windowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	LRESULT result = 0;
	
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY:
		running = false;
		break;
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		bufferWidth = rect.right - rect.left;
		bufferHeight = rect.bottom - rect.top;

		int bufferSize = bufferWidth * bufferHeight * sizeof(unsigned int);

		if (bufferMemory) VirtualFree(bufferMemory, 0, MEM_RELEASE);
		bufferMemory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		bufferBitmapInfo.bmiHeader.biSize = sizeof(bufferBitmapInfo.bmiHeader);
		bufferBitmapInfo.bmiHeader.biWidth = bufferWidth;
		bufferBitmapInfo.bmiHeader.biHeight = bufferHeight;
		bufferBitmapInfo.bmiHeader.biPlanes = 1;
		bufferBitmapInfo.bmiHeader.biBitCount = 32;
		bufferBitmapInfo.bmiHeader.biCompression = BI_RGB;

	}break;

	default: 
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	
	return result;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	//Create a window class
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = "Game Window Class";
	windowClass.lpfnWndProc = windowCallback;
	//register it
	RegisterClass(&windowClass);

	//create window
	HWND window = CreateWindow(windowClass.lpszClassName, "Pacman!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);
	while (running) {
		//input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		//simulate
		unsigned int* pixel = (unsigned int*)bufferMemory;
		for (int y = 0; y < bufferHeight; y++) {
			for (int x = 0; x < bufferWidth; x++) {
				*pixel++ = x * y;
			}
		}

		//render
		StretchDIBits(hdc, 0, 0, bufferWidth, bufferHeight, 0, 0, bufferWidth, bufferHeight, bufferMemory, &bufferBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}

}
