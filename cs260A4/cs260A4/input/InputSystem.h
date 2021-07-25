#pragma once

#include "KeyEnum.h"

class InputSystem
{
public:
	void Update();

private:
	unsigned char _currKeyState[256];
	unsigned char _prevKeyState[256];
	
};