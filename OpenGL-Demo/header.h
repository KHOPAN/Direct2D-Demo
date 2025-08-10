#pragma once

#include <d2d1.h>

#ifdef __cplusplus
extern "C" {
#endif
BOOLEAN InitializeDirect2D(ID2D1Factory** factory);
void ReleaseFactory(ID2D1Factory* factory);
void ResizeTarget(ID2D1HwndRenderTarget* target, LPARAM lparam);
#ifdef __cplusplus
}
#endif
