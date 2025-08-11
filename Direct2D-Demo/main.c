#include "header.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define CLASS_NAME L"Direct2DDemo"

#define SPEED 0.17

static HANDLE processHeap;

static double minimum(const double x, const double y) {
	return x < y ? x : y;
}

static LRESULT CALLBACK windowProcedure(const HWND window, const UINT message, const WPARAM wparam, const LPARAM lparam) {
	const PDATA data = (PDATA) GetWindowLongPtrW(window, GWLP_USERDATA);

	if(!data && message != WM_CREATE) {
		return DefWindowProcW(window, message, wparam, lparam);
	}

	RECT bounds;
	POINT location;

	switch(message) {
	case WM_CREATE:
		SetLastError(0);
		return SetWindowLongPtrW(window, GWLP_USERDATA, (LONG_PTR) ((LPCREATESTRUCTW) lparam)->lpCreateParams) ? 0 : GetLastError() ? -1 : 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		if((!data->target && (!GetClientRect(window, &bounds) || !CreateResources(data->factory, &data->target, &data->brush, window, bounds))) || !GetCursorPos(&location) || !ScreenToClient(window, &location)) {
			return 0;
		}

		data->x += (((double) location.x) - data->x) * SPEED;
		data->y += (((double) location.y) - data->y) * SPEED;
		const LONG deltaX = location.x - data->previous.x;
		const LONG deltaY = location.y - data->previous.y;
		data->previous.x = location.x;
		data->previous.y = location.y;
		const double velocity = minimum(sqrt(deltaX * deltaX + deltaY * deltaY) * 4.0, 150.0);
		data->scale += (velocity / 300.0 - data->scale) * SPEED;

		if(velocity > 20.0) {
			data->angle = 90.0 - atan2(deltaX, deltaY) * 180.0 / M_PI;
		}

		Render(data->target, data->brush, (float) data->angle, (float) data->x, (float) data->y, data->size, data->scale);
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
		goto freeResources;
	}

	const HWND window = CreateWindowExW(0L, CLASS_NAME, L"Direct2D Demo", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, instance, data);

	if(!window) {
		ERROR_DIALOG(L"CreateWindowExW() failed");
		goto unregisterClass;
	}

	const double scale = ((double) GetDpiForWindow(window)) / 96.0;
	data->size = scale * 50.0;
	const int width = (int) (scale * 600.0);
	const int height = (int) (scale * 400.0);

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
freeResources:
	FreeResources(data);
freeData:
	HeapFree(processHeap, 0, data);
uninitialize:
	CoUninitialize();
	return codeExit;
}
