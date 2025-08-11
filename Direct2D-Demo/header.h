#pragma once

#include <d2d1.h>

#ifdef __cplusplus
extern "C" {
#endif
BOOLEAN InitializeDirect2D(ID2D1Factory** factory);
void ReleaseFactory(ID2D1Factory* factory);
void ResizeTarget(ID2D1HwndRenderTarget* target, LPARAM lparam);
BOOLEAN CreateResources(ID2D1Factory* factory, HWND window, ID2D1HwndRenderTarget** target, ID2D1SolidColorBrush** brush);
void Render(ID2D1HwndRenderTarget* target, ID2D1SolidColorBrush* brush);
#ifdef __cplusplus
}
#endif
