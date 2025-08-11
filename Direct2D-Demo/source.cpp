#include "header.h"

BOOLEAN CreateResources(ID2D1Factory* const factory, ID2D1HwndRenderTarget** const target, ID2D1SolidColorBrush** const brush, const HWND window, const RECT bounds) {
	return SUCCEEDED(factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(window, D2D1::SizeU(bounds.right - bounds.left, bounds.bottom - bounds.top)), target)) && SUCCEEDED((*target)->CreateSolidColorBrush(D2D1::ColorF(0xFFFFFF, 1.0), brush));
}

void FreeResources(const PDATA data) {
	if(data->target) {
		data->target->Release();
	}

	if(data->brush) {
		data->brush->Release();
	}

	data->factory->Release();
}

BOOLEAN InitializeDirect2D(ID2D1Factory** const factory) {
	if(FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory))) {
		ERROR_DIALOG(L"D2D1CreateFactory() failed");
		return FALSE;
	}

	return TRUE;
}

void Render(ID2D1HwndRenderTarget* const target, ID2D1SolidColorBrush* const brush, const float angle, const float x, const float y, const double size, const double scale) {
	target->BeginDraw();
	target->Clear(D2D1::ColorF(0x101010, 1.0));
	target->SetTransform(D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(x, y)));
	target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), (float) (size * (1.0 + scale)), (float) (size * (1.0 - scale))), brush);

	if(target->EndDraw() == D2DERR_RECREATE_TARGET) {
		target->Release();
		brush->Release();
	}
}

void ResizeTarget(ID2D1HwndRenderTarget* const target, const LPARAM lparam) {
	target->Resize(D2D1::SizeU(LOWORD(lparam), HIWORD(lparam)));
}
