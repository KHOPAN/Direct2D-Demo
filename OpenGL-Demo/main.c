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

	ID2D1Factory* factory;
	int codeExit = 1;

	if(!InitializeDirect2D(&factory)) {
		goto uninitialize;
	}

	WNDCLASSEXW windowClass = {0};
	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.lpfnWndProc = windowProcedure;
	windowClass.hInstance = instance;
	windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	windowClass.lpszClassName = CLASS_NAME;

	if(!RegisterClassExW(&windowClass)) {
		goto releaseFactory;
	}

	HWND window = CreateWindowExW(0L, CLASS_NAME, L"Demo Direct2D", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, instance, NULL);

	if(!window) {
		goto unregisterClass;
	}

	double scale = ((double) GetDpiForWindow(window)) / 96.0;
	UINT width = (UINT) (600.0 * scale);
	UINT height = (UINT) (400.0 * scale);

	if(!SetWindowPos(window, HWND_TOP, (UINT) ((((double) GetSystemMetrics(SM_CXSCREEN)) - ((double) width)) / 2.0), (UINT) ((((double) GetSystemMetrics(SM_CYSCREEN)) - ((double) height)) / 2.0), width, height, 0)) {
		goto destroyWindow;
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
releaseFactory:
	ReleaseFactory(factory);
uninitialize:
	CoUninitialize();
	return codeExit;
}
