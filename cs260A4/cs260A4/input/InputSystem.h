#pragma once

#include "KeyEnum.h"

class InputSystem
{
public:
	void Update();
	// detect player press and release
	bool KeyPressed(KEY key) const;
	// detect player continue holding, will return true every time
	bool KeyHold(KEY key) const;
	
private:
	unsigned char _currKeyState[256];
	unsigned char _prevKeyState[256];
	
};