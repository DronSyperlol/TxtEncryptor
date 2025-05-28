#pragma once
#include "KeyboardKeys.h"

class KeyboardState {
private:
	bool _shift = false;
	bool _ctrl = false;
	
	void setKey(KeyboardKeys key, bool isPressed);

public:
	void keyDown(KeyboardKeys key);
	void keyUp(KeyboardKeys key);

	bool shiftPressed() const;
	bool ctrlPressed() const;
};