#pragma once
#include "ComponentBase.h"

class Button : public ComponentBase {
private:

	void (*_onClick)();

public:
	Button(HINSTANCE hInst, HWND parent, LPCWSTR name, int x, int y, int width, int height, void (*onClick)());

	void trigger(WPARAM wp, LPARAM lp) const override;
};