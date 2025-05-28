#pragma once

#include "WindowBase.h"


class MainWindow : public WindowBase {
private:
	static LPWNDCLASSEX _wndClass;

	LPCWSTR getClassName() const override;
	
	LRESULT onWindowCreate(WPARAM wp, LPARAM lp) override;
	LRESULT onWindowDestroy(WPARAM wp, LPARAM lp) override;
	LRESULT onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const override;

	LONG64 _edit_id = NULL;

public:
	MainWindow(HINSTANCE);
};