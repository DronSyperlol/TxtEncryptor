#pragma once
#include <Windows.h>

class ComponentBase {
private:
	static LRESULT routeEvents(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

	HWND initializeWindow(
		DWORD exStyle, LPCWSTR className, LPCWSTR windowName, DWORD style,
		int x, int y, int width, int height, int id
	) const;


public:
	ComponentBase(HINSTANCE, LPCWSTR className, HWND parent, int x, int y, int width, int height);
	virtual ~ComponentBase();

	void create(int id);
	virtual void trigger(WPARAM wp, LPARAM lp) const = 0;

protected:
	HINSTANCE _hInst;
	HWND _parent;
	LPCWSTR _className;
	POINT _point;
	SIZE _size;

	HWND _hWnd =			NULL;
	WNDPROC _defWndProc =	NULL;
	int _id =				NULL;
	
	virtual LRESULT onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const;
};

typedef ComponentBase* LPComponentBase;