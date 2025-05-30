#include "LoadDialog.h"

#include "Edit.h"
#include "Button.h"

LPWNDCLASSEX LoadDialog::_wndClass = nullptr;

LPCWSTR LoadDialog::getClassName() const
{
	if (_wndClass == nullptr) {
		_wndClass = new WNDCLASSEX;
		ZeroMemory(_wndClass, sizeof(WNDCLASSEX));
		_wndClass->cbSize = sizeof(WNDCLASSEX);
		_wndClass->hInstance = _hInst;
		_wndClass->lpszClassName = L"loadDialog";
		_wndClass->lpfnWndProc = WindowBase::routeEvents;
		_wndClass->hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
		if (!RegisterClassEx(_wndClass)) throw "Cannot register class";
	}
	return _wndClass->lpszClassName;
}

LRESULT LoadDialog::onWindowCreate(WPARAM wp, LPARAM lp)
{
	//_edit_pass_1_id = addComponent(new Edit(_hInst, _hWnd, 10, 10, 100, 50, ES_PASSWORD | WS_BORDER));
	_edit_pass_2_id = addComponent(new Edit(_hInst, _hWnd, 10, 110, 100, 50, ES_PASSWORD | WS_BORDER));
	_btn_cancel = addComponent(new Button(_hInst, _hWnd, L"Отменить", 10, 210, 100, 50, 
		[]() {
			MessageBox(NULL, L"Cancel", L"test", MB_OK);
		}));
	_btn_load		= addComponent(new Button(_hInst, _hWnd, L"Load", 110, 210, 100, 50,
		[]() {
			MessageBox(NULL, L"Load", L"test", MB_OK);
		}));
	EnableWindow(_parent, FALSE);
	return DefWindowProc(_hWnd, WM_CREATE, wp, lp);
}

LRESULT LoadDialog::onWindowDestroy(WPARAM wp, LPARAM lp)
{
	EnableWindow(_parent, TRUE);
	return DefWindowProc(_hWnd, WM_CREATE, wp, lp);
}

LRESULT LoadDialog::onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_CLOSE:
		destroyWindow();
		break;
	}
	return WindowBase::onRawWndProc(hWnd, msg, wp, lp);
}

LoadDialog::LoadDialog(HINSTANCE hInst, HWND parent) : WindowBase(hInst)
{ 
	_parent = parent;
	initializeWindow(NULL, L"Load", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400);
	show(true);
}


