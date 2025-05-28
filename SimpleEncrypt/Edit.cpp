#include "Edit.h"


Edit::Edit(HINSTANCE hInst, HWND parent, int x, int y, int width, int height) 
	: ComponentBase(hInst, L"edit", parent, x, y, width, height)
{ }

void Edit::trigger(WPARAM wp, LPARAM lp) const
{
	MessageBox(_parent, L"edit", L"trigger", MB_OK);
}
