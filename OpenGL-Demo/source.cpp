#include "header.h"

BOOLEAN InitializeDirect2D(ID2D1Factory** factory) {
	if(FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory))) {
		return FALSE;
	}

	return TRUE;
}

void ReleaseFactory(ID2D1Factory* factory) {
	factory->Release();
}

void ResizeTarget(ID2D1HwndRenderTarget* target, LPARAM lparam) {
	target->Resize(D2D1::SizeU(HIWORD(lparam), LOWORD(lparam)));
}

BOOLEAN CreateResources(ID2D1Factory* factory, HWND window, ID2D1HwndRenderTarget** target, ID2D1SolidColorBrush** brush) {
	RECT bounds;
	GetClientRect(window, &bounds);

	if(FAILED(factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(window, D2D1::SizeU(bounds.right - bounds.left, bounds.bottom - bounds.top)), target))) {
		return FALSE;
	}

	if(FAILED((*target)->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSkyBlue), brush))) {
		return FALSE;
	}

	return TRUE;
}

void Render(ID2D1HwndRenderTarget* target, ID2D1SolidColorBrush* brush) {
	target->BeginDraw();
	target->Clear(D2D1::ColorF(0xFF101010, 0.0));

	if(target->EndDraw() == D2DERR_RECREATE_TARGET) {
		target->Release();
		brush->Release();
	}
}
