#pragma once

#include <Windows.h>
#include <map>
#include "ComponentBase.h"

class WindowBase {
private: 
	std::map<LONG64, LPComponentBase> _childs;
public:

	WindowBase(HINSTANCE hInst);
	virtual ~WindowBase();
	
	virtual void show(bool isVisible) const;
	virtual void move(int x, int y) const;
	virtual void sendMessage(UINT msg, WPARAM wParam, LPARAM lParam) const;
	virtual void destroyWindow() const;


	LONG64 addChild(LPComponentBase child);
	void removeChild(LONG64 childId);

protected:
	HINSTANCE _hInst = NULL;
	HWND _hWnd = NULL;
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
	virtual LRESULT onWindowCreate(WPARAM, LPARAM) = 0;
	virtual LRESULT onWindowDestroy(WPARAM, LPARAM) = 0;
	virtual LRESULT onCommand(WPARAM, LPARAM) const;
};

typedef WindowBase* LPWindowBase;