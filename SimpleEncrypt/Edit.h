#pragma once
#include "ComponentBase.h"
#include "KeyboardState.h"

class Edit : public ComponentBase {
private:
	static constexpr int PADDING = 10;
	static constexpr int PADDING_END = PADDING * 2;

	LRESULT onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const override;
	KeyboardState* _kState;

	void size(int cx, int cy) const override;

public:
	Edit(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, DWORD additionalStyles);
	~Edit();
	void trigger(WPARAM wp, LPARAM lp) const override;

	static constexpr short ON_SHORTCUT = 1000;

protected: 
};