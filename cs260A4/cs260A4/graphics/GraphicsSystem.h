#pragma once
#include "../framework.h"
#include "../Math/Transform.h"


class GraphicsSystem
{
	HDC windowDC{}; // global Window-device context
	HGLRC wglDC{}; // OpenGL-device context

	HWND _hwnd; //reference to the handle to the window


	Transform objTransform{};
	Transform objTransform2{ Vector3(0, -1, 0) };
	Transform camTransform{ Vector3(0, 0, -10) };
	Transform lightTransform{ Vector3(-10, 0, 0) };

public:
	GraphicsSystem()
	{
	}

	void Init(HWND hwnd);
	void Update();
	void LateUpdate();
	void Exit();

	bool InitializeRenderingEnvironment();
	void CleanRenderingEnvironment();
};
