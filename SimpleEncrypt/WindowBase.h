#pragma once

#include <Windows.h>
#include <map>
#include "ComponentBase.h"

class WindowBase {
private: 
	std::map<LONG64, LPComponentBase> _components;
	std::map<HWND, WindowBase*> _childs;
public:

	WindowBase(HINSTANCE hInst);
	virtual ~WindowBase();
	
	virtual void show(bool isVisible) const;
	virtual void move(int x, int y) const;
	virtual void sendMessage(UINT msg, WPARAM wParam, LPARAM lParam) const;
	virtual void destroyWindow() const;

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
	virtual LRESULT onRawWndProc(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT onWindowCreate(WPARAM, LPARAM) = 0;
	virtual LRESULT onWindowDestroy(WPARAM, LPARAM) = 0;
	virtual LRESULT onCommand(WPARAM, LPARAM);

	LONG64 addComponent(LPComponentBase child);
	void removeComponent(LONG64 componentId);
	LPComponentBase getComponent(LONG64 componentId) const;

	HWND addChild(WindowBase* window);
	void removeChild(HWND hWnd);

};

typedef WindowBase* LPWindowBase;