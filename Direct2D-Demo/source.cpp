#include "header.h"

BOOLEAN InitializeDirect2D(ID2D1Factory** factory) {
	return SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory));
}

void ReleaseFactory(ID2D1Factory* factory) {
	factory->Release();
}

void ResizeTarget(ID2D1HwndRenderTarget* target, LPARAM lparam) {
	target->Resize(D2D1::SizeU(LOWORD(lparam), HIWORD(lparam)));
}

BOOLEAN CreateResources(ID2D1Factory* factory, HWND window, ID2D1HwndRenderTarget** target, ID2D1SolidColorBrush** brush) {
	RECT bounds;
	GetClientRect(window, &bounds);
	return SUCCEEDED(factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(window, D2D1::SizeU(bounds.right - bounds.left, bounds.bottom - bounds.top)), target)) && SUCCEEDED((*target)->CreateSolidColorBrush(D2D1::ColorF(0xFFFFFF, 1.0), brush));
}

void Render(ID2D1HwndRenderTarget* target, ID2D1SolidColorBrush* brush, double angle, double scale, double x, double y) {
	target->BeginDraw();
	target->Clear(D2D1::ColorF(0x101010, 1.0));
	target->SetTransform(D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(x, y)));
	target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), 50.0 * (1.0 + scale), 50.0 * (1.0 - scale)), brush);

	if(target->EndDraw() == D2DERR_RECREATE_TARGET) {
		target->Release();
		brush->Release();
	}
}
