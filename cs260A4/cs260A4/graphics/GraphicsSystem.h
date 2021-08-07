#pragma once
#include "../framework.h"
#include "../Math/Transform.h"
#include "../core/GameObject.h"
#include <vector>
#include "Shader.h"
#include "../core/Factory.h"
#include <gl/glew.h>

class GraphicsSystem
{
	HDC windowDC{}; // global Window-device context
	HGLRC wglDC{}; // OpenGL-device context

	HWND _hwnd; //reference to the handle to the window



	Transform objTransform{};
	Transform objTransform2{ Vector3(0, -1, 0) };
	Transform camTransform{ Vector3(0, 0, -10) };
	Transform lightTransform{ Vector3(-10, 0, 0) };

	void RenderGameObject(GameObject& gameobj);

	Shader _shader;
	GLuint _vaotri;
	GLuint _vaoquad;
	GLuint _vbotri;
	GLuint _vboquad;

	glm::mat4 proj;
	

public:
	GraphicsSystem()
	{
	}

	void Init(HWND hwnd);
	void Update(std::vector<GameObject>& gameobjlist);
	void Update(Factory* factory);
	void LateUpdate();
	void Exit();

	bool InitializeRenderingEnvironment();
	void CleanRenderingEnvironment();
};
