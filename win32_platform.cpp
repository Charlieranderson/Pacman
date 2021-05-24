/*
Pacman - A recreation made by Charlie Anderson
5/10/21

Here is a change
*/



#include <windows.h>
#include "utils.cpp";
global_variable bool running = true;

struct RenderState {
	int height, width;
	void *memory;

	BITMAPINFO bitmapInfo;
};

global_variable RenderState renderState;

#include "platform_common.cpp"
#include "Rend.cpp"
#include "game.cpp"


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
		renderState.width = rect.right - rect.left;
		renderState.height = rect.bottom - rect.top;

		int bufferSize = renderState.width * renderState.height * sizeof(unsigned int);

		if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE);
		renderState.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
		renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
		renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
		renderState.bitmapInfo.bmiHeader.biPlanes = 1;
		renderState.bitmapInfo.bmiHeader.biBitCount = 32;
		renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;

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

	Input input = {};

	float deltaTime = 0.0166666f;
	LARGE_INTEGER frameBeginTime; 
	QueryPerformanceCounter(&frameBeginTime);

	float performanceFrequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performanceFrequency = (float)perf.QuadPart;
	}

	while (running) {
		//input
		MSG message;

		for (int i = 0; i < BUTTONCOUNT; i++) {
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

			switch (message.message) {
			case WM_KEYUP:
			case WM_KEYDOWN: {


				u32 vkCode = (u32)message.wParam;
				bool isDown = ((message.lParam & (1 << 31)) == 0);

#define processButton(b, vk)\
case vk: {\
input.buttons[b].isDown = isDown;\
input.buttons[b].changed = true;\
} break;

				switch (vkCode) {
					processButton(BUTTONUP, VK_UP);
					processButton(BUTTONDOWN, VK_DOWN);
					processButton(BUTTONLEFT, VK_LEFT);
					processButton(BUTTONRIGHT, VK_RIGHT);
				}
			}break;

			default:
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}

		//simulate
		simulateGame(&input, deltaTime);

		//render
		StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	

		LARGE_INTEGER frameEndTime;
		QueryPerformanceCounter(&frameEndTime);
		deltaTime = (float)(frameEndTime.QuadPart - frameBeginTime.QuadPart) / performanceFrequency;
		frameBeginTime = frameEndTime;
	}

}
