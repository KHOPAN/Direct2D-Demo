#include "header.h"

BOOLEAN InitializeDirect2D(ID2D1Factory** factory) {
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory);

	if(FAILED(result)) {
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
