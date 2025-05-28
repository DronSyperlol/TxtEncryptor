#include "Edit.h"
#include "KeyboardKeys.h"
#include "Shortcuts.h"

LRESULT Edit::onRawWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) const
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		int key = LOWORD(wp);
		switch (key)
		{
		case KeyboardKeys::Key_S:
			if (_kState->ctrlPressed() && !_kState->shiftPressed())
				SendMessage(_parent, WM_USER, Edit::ON_SHORTCUT | Shortcuts::Save << 16, _id);
			break;
		default:
			_kState->keyDown(static_cast<KeyboardKeys>(key));
			break;
		}
	}
	break;
	case WM_KEYUP:
	{
		int key = LOWORD(wp);
		switch (key)
		{
		default:
			_kState->keyUp(static_cast<KeyboardKeys>(key));
			break;
		}
	}
	break;
	default:
		break;
	}
	return ComponentBase::onRawWndProc(hWnd, msg, wp, lp);
}

void Edit::size(int cx, int cy) const
{
	ComponentBase::size(cx - PADDING_END, cy - PADDING_END);
}

Edit::Edit(HINSTANCE hInst, HWND parent, int x, int y, int width, int height, DWORD additionalStyles)
	: ComponentBase(hInst, L"edit", parent, x + PADDING, y + PADDING, width - PADDING_END, height - PADDING_END)
{
	_kState = new KeyboardState();
	_additionalStyles = additionalStyles;
}

Edit::~Edit()
{
	delete _kState;
}

void Edit::trigger(WPARAM wp, LPARAM lp) const
{
}
