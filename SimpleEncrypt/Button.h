#pragma once
#include "ComponentBase.h"

class Button : public ComponentBase {
private:

public:
	Button(HINSTANCE hInst, HWND parent, int x, int y, int width, int height);

	void trigger(WPARAM wp, LPARAM lp) const override;
};