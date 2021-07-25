#pragma once

#include <Windows.h>
#include "../framework.h"
#include "../Resource.h"

#define MAX_LOADSTRING 100

class WindowSystem
{
public:
	WindowSystem(HINSTANCE hInstance, int nCmdShow, unsigned width, unsigned height);
	void Update(bool& isRunning);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE, int);
private:
	HINSTANCE _hInstance;
	unsigned  _width;
	unsigned  _height;
	int       _nCmdShow;

	WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
	WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
};

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);