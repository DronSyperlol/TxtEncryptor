#include "KeyboardState.h"

void KeyboardState::setKey(KeyboardKeys key, bool isPressed)
{
	switch (key)
	{
	case KeyboardKeys::Ctrl:
		_ctrl = isPressed;
		break;
	case KeyboardKeys::Shift:
		_shift = isPressed;
		break;
	}
}

void KeyboardState::keyDown(KeyboardKeys key)
{
	setKey(key, true);
}

void KeyboardState::keyUp(KeyboardKeys key)
{
	setKey(key, false);
}

bool KeyboardState::shiftPressed() const
{
	return _shift;
}

bool KeyboardState::ctrlPressed() const
{
	return _ctrl;
}
