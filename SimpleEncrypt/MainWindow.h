#pragma once

#include "WindowBase.h"


class MainWindow : public WindowBase {
private:
	static LPWNDCLASSEX _wndClass;

	LPCWSTR getClassName() const override;
	
	LRESULT onWindowCreate(WPARAM wp, LPARAM lp) override;
	LRESULT onWindowDestroy(WPARAM wp, LPARAM lp) override;

public:
	MainWindow(HINSTANCE);
};