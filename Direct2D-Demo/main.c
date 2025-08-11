#define _USE_MATH_DEFINES
#include "header.h"
#include <math.h>

#define CLASS_NAME L"Direct2DDemo"

typedef struct {
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* target;
	ID2D1SolidColorBrush* brush;
	double circleX;
	double circleY;
	POINT previousMouse;
	double scale;
	double angle;
} DATA, *PDATA;

static HANDLE processHeap;

static double minimum(const double x, const double y) {
	return x < y ? x : y;
}

static LRESULT CALLBACK windowProcedure(const HWND window, const UINT message, const WPARAM wparam, const LPARAM lparam) {
	PDATA data = (PDATA) GetWindowLongPtrW(window, GWLP_USERDATA);

	if(!data && message != WM_CREATE) {
		return DefWindowProcW(window, message, wparam, lparam);
	}

	switch(message) {
	case WM_CREATE:
		SetLastError(0);
		return SetWindowLongPtrW(window, GWLP_USERDATA, (LONG_PTR) ((LPCREATESTRUCTW) lparam)->lpCreateParams) ? 0 : GetLastError() ? -1 : 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_DISPLAYCHANGE:
		InvalidateRect(window, NULL, 0);
		return 0;
	case WM_PAINT:
		if(!data->target && !CreateResources(data->factory, window, &data->target, &data->brush)) {
			return 0;
		}

		POINT location;

		if(!GetCursorPos(&location)) {
			return 0;
		}

		ScreenToClient(window, &location);
		double speed = 0.17;
		data->circleX += (((double) location.x) - data->circleX) * speed;
		data->circleY += (((double) location.y) - data->circleY) * speed;
		int deltaMouseX = location.x - data->previousMouse.x;
		int deltaMouseY = location.y - data->previousMouse.y;
		data->previousMouse.x = location.x;
		data->previousMouse.y = location.y;
		double velocity = minimum(sqrt(((double) deltaMouseX) * ((double) deltaMouseX) + ((double) deltaMouseY) * ((double) deltaMouseY)) * 4.0, 150.0);
		double scale = velocity / 150.0 * 0.5;
		data->scale += (scale - data->scale) * speed;
		double angle = (90.0 - (atan2(deltaMouseX, deltaMouseY) * 180.0 / M_PI));

		if(velocity > 20.0) {
			data->angle = angle;
		}

		Render(data->target, data->brush, data->angle, data->scale, data->circleX, data->circleY);
		InvalidateRect(window, NULL, 0);
		return 0;
	case WM_SIZE:
		if(data->target) {
			ResizeTarget(data->target, lparam);
		}

		return 0;
	}

	return DefWindowProcW(window, message, wparam, lparam);
}

int WINAPI WinMain(_In_ const HINSTANCE instance, _In_opt_ const HINSTANCE previousInstance, _In_ const LPSTR command, _In_ const int show) {
	if(!HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0)) {
		ERROR_DIALOG(L"HeapSetInformation() failed");
		return 1;
	}

	if(FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
		ERROR_DIALOG(L"CoInitializeEx() failed");
		return 1;
	}

	const PDATA data = HeapAlloc(processHeap = GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DATA));
	int codeExit = 1;

	if(!data) {
		ERROR_DIALOG(L"HeapAlloc() failed");
		goto uninitialize;
	}

	if(!InitializeDirect2D(&data->factory)) {
		goto freeData;
	}

	WNDCLASSEXW windowClass = {0};
	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = windowProcedure;
	windowClass.hInstance = instance;
	windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	windowClass.lpszClassName = CLASS_NAME;

	if(!RegisterClassExW(&windowClass)) {
		ERROR_DIALOG(L"RegisterClassExW() failed");
		goto releaseFactory;
	}

	const HWND window = CreateWindowExW(0L, CLASS_NAME, L"Direct2D Demo", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, instance, data);

	if(!window) {
		ERROR_DIALOG(L"CreateWindowExW() failed");
		goto unregisterClass;
	}

	const double scale = ((double) GetDpiForWindow(window)) / 96.0;
	const int width = (int) (600.0 * scale);
	const int height = (int) (400.0 * scale);

	if(!SetWindowPos(window, HWND_TOP, (int) ((((double) GetSystemMetrics(SM_CXSCREEN)) - ((double) width)) / 2.0), (int) ((((double) GetSystemMetrics(SM_CYSCREEN)) - ((double) height)) / 2.0), width, height, 0)) {
		ERROR_DIALOG(L"SetWindowPos() failed");
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
	ReleaseFactory(data->factory);
freeData:
	HeapFree(processHeap, 0, data);
uninitialize:
	CoUninitialize();
	return codeExit;
}
