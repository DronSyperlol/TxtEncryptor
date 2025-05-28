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
		if (!RegisterClassEx(_wndClass)) throw "Cannot register class";
	}
	return _wndClass->lpszClassName;
}

LRESULT MainWindow::onWindowCreate(WPARAM wp, LPARAM lp)
{
	const int padding = 10;
	RECT rc = { 0 };
	GetWindowRect(_hWnd, &rc);
	SIZE size = { rc.right - rc.left, rc.bottom - rc.top };
	/*addChild(
		new Edit(
		_hInst, 
		_hWnd,
		padding, padding, 
		size.cx - padding * 2, size.cy - padding * 2)
	);*/
	addChild(
		new Button(_hInst, _hWnd, 10, 10, 100, 50)
	);
	addChild(
		new Button(_hInst, _hWnd, 10, 70, 100, 50)
	);
	return DefWindowProc(_hWnd, WM_CREATE, wp, lp);
}

LRESULT MainWindow::onWindowDestroy(WPARAM wp, LPARAM lp)
{
	PostQuitMessage(0);
	return DefWindowProc(_hWnd, WM_DESTROY, wp, lp);
}

MainWindow::MainWindow(HINSTANCE hInst) : WindowBase(hInst)
{
	initializeWindow(NULL, L"TxtEncryptor", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500);
	show(true);
}
