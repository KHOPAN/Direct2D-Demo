#pragma once

#include <d2d1.h>

#define ERROR_DIALOG(message) MessageBoxW(NULL, message, L"Direct2D Demo", MB_OK | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL)

typedef struct {
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* target;
	ID2D1SolidColorBrush* brush;
	double x;
	double y;
	POINT previous;
	double scale;
	double angle;
} DATA, *PDATA;

#ifdef __cplusplus
extern "C" {
#endif
BOOLEAN CreateResources(ID2D1Factory* const factory, ID2D1HwndRenderTarget** const target, ID2D1SolidColorBrush** const brush, const HWND window, const RECT bounds);
void FreeResources(const PDATA data);
BOOLEAN InitializeDirect2D(ID2D1Factory** const factory);
void Render(ID2D1HwndRenderTarget* const target, ID2D1SolidColorBrush* const brush, const float angle, const float x, const float y, const double size, const double scale);
void ResizeTarget(ID2D1HwndRenderTarget* const target, const LPARAM lparam);
#ifdef __cplusplus
}
#endif
