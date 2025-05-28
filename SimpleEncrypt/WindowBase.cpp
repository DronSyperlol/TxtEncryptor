#include "WindowBase.h"

WindowBase::WindowBase(HINSTANCE hInst)
{
	_hInst = hInst;
}

WindowBase::~WindowBase()
{ }

void WindowBase::show(bool isVisible) const
{
	ShowWindowAsync(_hWnd, isVisible ? SW_SHOW : SW_HIDE);
}

void WindowBase::move(int x, int y) const
{
	SetWindowPos(_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
}

void WindowBase::sendMessage(UINT msg, WPARAM wParam, LPARAM lParam) const
{
	SendMessage(_hWnd, msg, wParam, lParam);
}

void WindowBase::destroyWindow() const
{
	DestroyWindow(_hWnd);
}


LONG64 WindowBase::addComponent(LPComponentBase child)
{
	LONG64 id = _components.size() + 1;
	_components[id] = child;
	child->create(id);
	return id;
}

void WindowBase::removeComponent(LONG64 childId)
{
	_components.erase(childId);
}

LPComponentBase WindowBase::getComponent(LONG64 componentId) const
{
	if (_components.find(componentId) != _components.end())
		return _components.at(componentId);
	else
		return nullptr;
}

HWND WindowBase::initializeWindow(
	DWORD exStyle, LPCWSTR windowName, DWORD style, 
	int x, int y, int width, int height) const
{
	return CreateWindowEx(
		exStyle, getClassName(), windowName, style, 
		x, y, width, height, 
		_parent, _hMenu, _hInst, static_cast<LPVOID>(const_cast<LPWindowBase>(this)));
}

LRESULT WindowBase::routeEvents(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	LPWindowBase wnd = reinterpret_cast<LPWindowBase>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	try {
		if (wnd == nullptr && (msg == WM_NCCREATE || msg == WM_CREATE))
		{
			LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lp);
			wnd = static_cast<LPWindowBase>(cs->lpCreateParams);
			wnd->_hWnd = hWnd;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));
		}
	}
	catch (...) { }
	if (wnd == nullptr) return DefWindowProc(hWnd, msg, wp, lp);
	switch (msg)
	{
	case WM_CREATE:
		return wnd->onWindowCreate(wp, lp);
	case WM_DESTROY:
		return wnd->onWindowDestroy(wp, lp);
	case WM_COMMAND:
		return wnd->onCommand(wp, lp);
	default:
		return wnd->onRawWndProc(hWnd, msg, wp, lp);
	}
}

LRESULT WindowBase::onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const
{
	return DefWindowProc(hWnd, msg, wp, lp);
}

LRESULT WindowBase::onCommand(WPARAM wp, LPARAM lp) const
{
	int id = LOWORD(wp);
	auto component = getComponent(id);
	if (component != nullptr) component->trigger(wp, lp);
	return onRawWndProc(_hWnd, WM_COMMAND, wp, lp);
}

