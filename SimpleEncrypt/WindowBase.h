#pragma once

#include <Windows.h>
#include <map>

class WindowBase {
private: 
	std::map<LONG64, LPWindowBase> _childs;
public:
	WindowBase(HINSTANCE hInst);

	virtual void show(bool isVisible) const;
	virtual void move(int x, int y) const;
	virtual void sendMessage(UINT msg, WPARAM wParam, LPARAM lParam) const;
	virtual void destroyWindow() const;

	LONG64 addChild(WindowBase&& child);
	void removeChild(LONG64 childId);

	virtual ~WindowBase();
protected:
	HINSTANCE _hInst;
	HWND _hWnd;
	HWND _parent = NULL;
	HMENU _hMenu = NULL;

	virtual LPCWSTR getClassName() const = 0;
	HWND initializeWindow(
		DWORD exStyle, LPCWSTR windowName, DWORD style,
		int x, int y, int width, int height
	) const;

	static LRESULT routeEvents(HWND, UINT, WPARAM, LPARAM);

	// Events: 
	virtual LRESULT onRawWndProc(HWND, UINT, WPARAM, LPARAM) const;
	virtual LRESULT onWindowCreate(WPARAM, LPARAM) const = 0;
	virtual LRESULT onWindowDestroy(WPARAM, LPARAM) const = 0;
	virtual LRESULT onCommand(WPARAM, LPARAM) const;
};

typedef WindowBase* LPWindowBase;