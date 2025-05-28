#pragma once
#include "ComponentBase.h"
#include "KeyboardState.h"

class Edit : public ComponentBase {
private:
	static const int PADDING = 10;
	static const int PADDING_END = PADDING * 2;

	LRESULT onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const override;
	KeyboardState* _kState;

	void size(int cx, int cy) const override;

public:
	Edit(HINSTANCE hInst, HWND parent, int x, int y, int width, int height);
	~Edit();
	void trigger(WPARAM wp, LPARAM lp) const override;

protected: 
};