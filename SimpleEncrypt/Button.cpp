#include "Button.h"


Button::Button(HINSTANCE hInst, HWND parent, LPCWSTR name, int x, int y, int width, int height, void (*onClick)())
	: ComponentBase(hInst, L"button", parent, x, y, width, height)
{ 
	_name = name;
	_onClick = onClick;
}

void Button::trigger(WPARAM wp, LPARAM lp) const
{
	_onClick();
	//MessageBox(_parent, _id == 1 ? L"first button" : L"other button", L"trigger", MB_OK);
}
