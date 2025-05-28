#include "MainWindow.h"
#include "resource.h"
#include "Edit.h"
#include "Button.h"

LPWNDCLASSEX MainWindow::_wndClass = nullptr;

LPCWSTR MainWindow::getClassName() const
{
	if (_wndClass == nullptr) {
		_wndClass = new WNDCLASSEX;
		ZeroMemory(_wndClass, sizeof(WNDCLASSEX));
		_wndClass->cbSize = sizeof(WNDCLASSEX);
		_wndClass->hInstance = _hInst;
		_wndClass->lpszClassName = L"mainWindow";
		_wndClass->lpfnWndProc = WindowBase::routeEvents;
		_wndClass->hIcon = LoadIcon(_hInst, MAKEINTRESOURCE(IDI_ICON1));
		_wndClass->hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
		if (!RegisterClassEx(_wndClass)) throw "Cannot register class";
	}
	return _wndClass->lpszClassName;
}

LRESULT MainWindow::onWindowCreate(WPARAM wp, LPARAM lp)
{
	RECT rc = { 0 };
	GetWindowRect(_hWnd, &rc);
	SIZE size = { rc.right - rc.left, rc.bottom - rc.top };
	_edit_id = addComponent(new Edit(_hInst, _hWnd, 0, 0, size.cx, size.cy));
	return DefWindowProc(_hWnd, WM_CREATE, wp, lp);
}

LRESULT MainWindow::onWindowDestroy(WPARAM wp, LPARAM lp)
{
	PostQuitMessage(0);
	return DefWindowProc(_hWnd, WM_DESTROY, wp, lp);
}

LRESULT MainWindow::onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const
{
	switch (msg) {
	case WM_SIZE:
		getComponent(_edit_id)->size(LOWORD(lp), HIWORD(lp));
		break;
	}
	return WindowBase::onRawWndProc(hWnd, msg, wp, lp);
}

MainWindow::MainWindow(HINSTANCE hInst) : WindowBase(hInst)
{
	show(true);
}
