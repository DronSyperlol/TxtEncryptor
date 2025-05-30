#pragma once
#include "WindowBase.h"

class LoadDialog : public WindowBase
{
private:
	static LPWNDCLASSEX _wndClass;

	LPCWSTR getClassName() const override;

	LRESULT onWindowCreate(WPARAM wp, LPARAM lp) override;
	LRESULT onWindowDestroy(WPARAM wp, LPARAM lp) override;
	LRESULT onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) override;
	//LRESULT onCommand(WPARAM wp, LPARAM lp) override;

	int _edit_pass_1_id = 0;
	int _edit_pass_2_id = 0;

	int _btn_load = 0;
	int _btn_cancel = 0;

public:
	LoadDialog(HINSTANCE, HWND parent);

protected:
};

