#pragma once
#include "ComponentBase.h"


class Edit : public ComponentBase {
private:

public:

	Edit(HINSTANCE hInst, HWND parent, int x, int y, int width, int height);

	void trigger(WPARAM wp, LPARAM lp) const override;
};