#include "WindowSystem.h"
#include "../Resource.h"


WindowSystem::WindowSystem(HINSTANCE hInstance, int nCmdShow, unsigned width, unsigned height) :
	_hInstance{ hInstance },
	_nCmdShow{ nCmdShow },
	_width{ width },
	_height{ height }
{
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CS260A4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	InitInstance(hInstance, nCmdShow);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CS260A4));
}

void WindowSystem::Update(bool& isRunning)
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			isRunning = false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

ATOM WindowSystem::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CS260A4));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL WindowSystem::InitInstance(HINSTANCE, int)
{
	 _hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		200, 100, _width, _height, nullptr, nullptr, _hInstance, nullptr);

	if (!_hwnd)
	{
		return FALSE;
	}

	ShowWindow(_hwnd, _nCmdShow);
	UpdateWindow(_hwnd);

	return TRUE;
}

HWND WindowSystem::GetHandle() const
{
	return _hwnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}