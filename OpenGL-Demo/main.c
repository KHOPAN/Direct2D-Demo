#include "header.h"

#define CLASS_NAME L"DemoDirect2DClassName"

static LRESULT CALLBACK windowProcedure(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	switch(message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(window, message, wparam, lparam);
}

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE previousInstance, _In_ LPSTR command, _In_ int show) {
	if(!HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0)) {
		return 1;
	}

	HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if(FAILED(result)) {
		return 1;
	}

	if(!InitializeDirect2D()) {
		goto uninitialize;
	}
uninitialize:
	CoUninitialize();
	return 0;
	/*WNDCLASSEXW windowClass = {0};
	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.lpfnWndProc = windowProcedure;
	windowClass.hInstance = instance;
	windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	windowClass.lpszClassName = CLASS_NAME;

	if(!RegisterClassExW(&windowClass)) {
		return 1;
	}

	HWND window = CreateWindowExW(0L, CLASS_NAME, L"Demo Direct2D", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, NULL, instance, NULL);
	int codeExit = 1;

	if(!window) {
		goto unregisterClass;
	}

	ShowWindow(window, show);
	MSG message;

	while(GetMessageW(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}

	codeExit = 0;
destroyWindow:
	DestroyWindow(window);
unregisterClass:
	UnregisterClassW(CLASS_NAME, instance);
	return codeExit; */
}
