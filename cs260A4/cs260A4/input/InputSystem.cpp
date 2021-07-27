#include "InputSystem.h"
#include <Windows.h>
#include <algorithm>

void InputSystem::Update()
{
	std::swap(_currKeyState, _prevKeyState);
	// get keyboard state
	BOOL success = GetKeyboardState(_currKeyState);
}

bool InputSystem::KeyPressed(KEY key)
{

	if ((_currKeyState[key] & 0xF0) && !(_prevKeyState[key] & 0xF0))
		return true;
	return false;

	if (_currKeyState[key] & 0xF0)
		return true;
	return false;
}

bool InputSystem::KeyHold(KEY key)
{
	if (_currKeyState[key] & 0xF0)
		return true;
	return false;
}
