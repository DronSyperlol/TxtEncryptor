#include "ComponentBase.h"

ComponentBase::ComponentBase(
	HINSTANCE hInst, LPCWSTR className, HWND parent,
	int x, int y, int width, int height)
{
	_hInst = hInst;
	_parent = parent;
	_point = { x, y };
	_size = { width, height };
	_className = className;
}

ComponentBase::~ComponentBase()
{ }

void ComponentBase::create(int id)
{
	_id = id;
	_hWnd = initializeWindow(NULL, _className, L"", WS_VISIBLE | WS_CHILD | _additionalStyles,
		_point.x, _point.y, _size.cx, _size.cy, id);
	SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	_defWndProc = (WNDPROC)SetWindowLongPtr(_hWnd, GWLP_WNDPROC, (LONG_PTR)ComponentBase::routeEvents);
}

void ComponentBase::move(int x, int y) const
{
	SetWindowPos(_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
}

void ComponentBase::size(int cx, int cy) const
{
	SetWindowPos(_hWnd, NULL, 0, 0, cx, cy, SWP_NOMOVE);
}

HWND ComponentBase::initializeWindow(
	DWORD exStyle, LPCWSTR className, LPCWSTR windowName, DWORD style,
	int x, int y, int width, int height, int id) const
{
	return CreateWindowEx(
		exStyle, className, windowName, style,
		x, y, width, height,
		_parent, (HMENU)id, _hInst, NULL);
}

LRESULT ComponentBase::routeEvents(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	LPComponentBase wnd = reinterpret_cast<LPComponentBase>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	switch (msg)
	{
	default:
		return wnd->onRawWndProc(hWnd, msg, wp, lp);
	}
}

LRESULT ComponentBase::onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const
{
	return CallWindowProc(_defWndProc, hWnd, msg, wp, lp);
}