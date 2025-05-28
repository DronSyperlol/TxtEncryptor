#include "Button.h"


Button::Button(HINSTANCE hInst, HWND parent, int x, int y, int width, int height) 
	: ComponentBase(hInst, L"button", parent, x, y, width, height)
{ }

void Button::trigger(WPARAM wp, LPARAM lp) const
{
	MessageBox(_parent, _id == 1 ? L"first button" : L"other button", L"trigger", MB_OK);
}
