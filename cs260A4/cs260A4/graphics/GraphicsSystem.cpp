#include "GraphicsSystem.h"
//#include "../Dep/glew/GL/glew.h"
#include "OpenGLErrorCheck.h"

#include "Mesh.h"
#include "Shader.h"

//#include "Graphics3d.h"
//#include "Camera.h"
//#include "Input/KeyboardInput.h"

#include "../Dep/glm/glm.hpp"
#include "../Dep/glm/gtc/matrix_transform.hpp"
#include "../Dep/glm/gtc/type_ptr.hpp"

void GraphicsSystem::RenderGameObject(GameObject& gameobj)
{
	_shader.use();

	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 view = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(gameobj.transform.position.x, gameobj.transform.position.y, 1));
	model = glm::rotate(model, gameobj.transform.rotation, glm::vec3(0.0f, 0.0f, 1));
	model = glm::scale(model, glm::vec3(gameobj.transform.scale.x, gameobj.transform.scale.y, 1));

	glm::mat4 mvp = proj * view * model;
	_shader.use();
	_shader.setMat4("u_MVP", mvp);
	// triangle mesh
	if (gameobj.obj_type == TYPE_PLAYER)
	{

		switch (gameobj.playerIndex)
		{
		case 0:
			_shader.setVec3("u_Color", 1, 0, 0);
			break;
		case 1:
			_shader.setVec3("u_Color", 0, 1, 0);
			break;
		case 2:
			_shader.setVec3("u_Color", 0, 0, 1);
			break;
		case 3:
			_shader.setVec3("u_Color", 1, 1, 0);
			break;
		}
		glBindVertexArray(_vaotri);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	// quad mesh
	else
	{
		glBindVertexArray(_vaoquad);

		// set color base on bullet, asteriod 
		if (gameobj.obj_type == TYPE_ASTEROID)
			_shader.setVec3("u_Color", 0.5, 0.1, 1);
		if (gameobj.obj_type == TYPE_BULLET)
			_shader.setVec3("u_Color", 0.5, 0.3, 0.05);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

}

void GraphicsSystem::Init(HWND hwnd)
{
	_hwnd = hwnd;

	std::cerr << "Initializing Rendering Environment..." << std::endl;
	InitializeRenderingEnvironment();

	glClearColor(0.5f, 0, 1, 1); //RGBA

	glewExperimental = GL_TRUE;

	//initialise glew
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "glewInit() failed!" << std::endl;
	}
	else
	{
		std::cerr << "Initializing OpenGL successful" << std::endl;
		std::cerr << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	}

	//Mesh::Init();

	//load in the default screen shader
	/*Shader::screenShader = new Shader("Asset/Shaders/DefaultScreen");

	Shader::defaultShader = new Shader("Asset/Shaders/Default.vs", "Asset/Shaders/Default.fs");*/
	//Graphics::Camera::camera = new Graphics::Camera;
	//Input::KeyboardInput::inst = new Input::KeyboardInput;

	RECT rect;
	GetClientRect(_hwnd, &rect);
	glViewport(static_cast<GLint>(0), static_cast<GLint>(0), rect.right - rect.left, rect.bottom - rect.top);

	proj = glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, -10.0f, 10.0f);

	glGenVertexArrays(1, &_vaotri);
	glBindVertexArray(_vaotri);

	glGenBuffers(1, &_vbotri);

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, 0.0f, 0.0f,
	 -0.5f,  0.5f, 0.0f
	};

	_shader.Init("Default.vs", "Default.fs");
	glBindBuffer(GL_ARRAY_BUFFER, _vbotri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glGenVertexArrays(1, &_vaoquad);
	glBindVertexArray(_vaoquad);

	glGenBuffers(1, &_vboquad);

	float vertices2[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f,  0.5f, 0.0f,  // top left 
	// second triangle
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left
	};

	glBindBuffer(GL_ARRAY_BUFFER, _vboquad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// glEnable(GL_DEPTH_TEST);
}

void GraphicsSystem::Update(std::vector<GameObject>& gameobjlist)
{

	//update opengl

	glClearColor(0, 0, 0, 1); //RGBA

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//_shader.use();
	//



	//// draw triangle mesh




	//glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//glm::mat4 view = glm::mat4(1.0f);
	//
	//model = glm::translate(model, glm::vec3(-200, 10, 1));
	//model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1));
	//model = glm::scale(model, glm::vec3(100, 100, 1));
	//
	//glm::mat4 mvp =  proj * model;
	//

	//_shader.setMat4("u_MVP", mvp);
	//_shader.setVec3("u_Color", 1,0,0);

	//glBindVertexArray(_vaotri);



	//glDrawArrays(GL_TRIANGLES, 0, 3);



	//// draw quad mesh

	//glBindVertexArray(_vaoquad);


	//model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	//view = glm::mat4(1.0f);

	//model = glm::translate(model, glm::vec3(100, 10, 1));
	//model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1));
	//model = glm::scale(model, glm::vec3(100, 100, 1));

	// mvp = proj * model;

	//_shader.use();
	//_shader.setMat4("u_MVP", mvp);
	//_shader.setVec3("u_Color", 0, 1, 0);

	//glDrawArrays(GL_TRIANGLES, 0, 6);



	for (auto& gameobj : gameobjlist)
	{
		if (gameobj.isActive == false)
			continue;
		RenderGameObject(gameobj);
	}



}

void GraphicsSystem::Update()
{
	// input
	//Input::KeyboardInput::inst->Update();
	//if (Input::KeyboardInput::inst->IsKeyTriggered(VK_1))
	//  Camera::camera->perspective = false;
	//if (Input::KeyboardInput::inst->IsKeyTriggered(VK_2))
	//  Camera::camera->perspective = true;
	//
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_A))
	//  camTransform.position += camTransform.Right() / 200;
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_D))
	//  camTransform.position += camTransform.Left() / 200;
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_S))
	//  camTransform.position += camTransform.Back() / 200;
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_W))
	//  camTransform.position += camTransform.Forward() / 200;
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_Q))
	//  camTransform.position += camTransform.Down() / 200;
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_E))
	//  camTransform.position += camTransform.Up() / 200;
	//
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_J))
	//  objTransform.position.x += -.001953125f;
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_L))
	//  objTransform.position.x += .001953125f;
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_K))
	//  objTransform.position.y += -.001953125f;
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_I))
	//  objTransform.position.y += .001953125f;
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_U))
	//  objTransform.position.z += -.001953125f;
	//if (Input::KeyboardInput::inst->IsKeyPressed(VK_O))
	//  objTransform.position.z += .001953125f;

	//static Vector3 euler1 = Vector3();
	//static Vector3 euler2 = Vector3();

	////if (Input::KeyboardInput::inst->IsKeyPressed(VK_7))
	////  euler1.y += -0.125f;
	////if (Input::KeyboardInput::inst->IsKeyPressed(VK_8))
	////  euler1.y += 0.125f;
	////if (Input::KeyboardInput::inst->IsKeyPressed(VK_9))
	////  euler2.y += -0.125f;
	////if (Input::KeyboardInput::inst->IsKeyPressed(VK_0))
	////  euler2.y += 0.125f;
	////
	//camTransform.rotation = Quaternion::EulerAnglesToQuaternion(euler1);
	//objTransform.rotation = Quaternion::EulerAnglesToQuaternion(euler2);
	////
	////if (Input::KeyboardInput::inst->IsKeyPressed(VK_3))
	////  camTransform.rotation = Quaternion::Identity();
	////if (Input::KeyboardInput::inst->IsKeyPressed(VK_4))
	////  objTransform.rotation = Quaternion::Identity();

	//euler1 = Quaternion::QuaternionToEulerAngles(camTransform.rotation);
	//euler2 = Quaternion::QuaternionToEulerAngles(objTransform.rotation);

	//// test mesh draw
	//Graphics::Camera::camera->Begin();
	//
	//Matrix4x4 proj = Graphics::Camera::camera->GetProjectionMatrix();
	//Matrix4x4 view = Graphics::Camera::camera->GetViewMatrix(camTransform);
	//Matrix4x4 model = objTransform.LocalToWorldMatrix();
	//
	///*Shader::defaultShader->SetUniformMatrix4f("_M", model);
	//Shader::defaultShader->SetUniformMatrix4f("_V", view);
	//Shader::defaultShader->SetUniformMatrix4f("_P", proj);
	//Shader::defaultShader->SetUniformVector3f("lightPos", camTransform.position);
	//Shader::defaultShader->SetUniformVector3f("lightPos", camTransform.position);
	//Shader::defaultShader->SetUniformVector3f("lightPos", camTransform.position);
	//Shader::defaultShader->SetUniformVector3f("camPos", camTransform.position);
	//Mesh::DrawCube(-50.0);

	//model = objTransform2.LocalToWorldMatrix();
	//Shader::defaultShader->SetUniformMatrix4f("_M", model);
	//Mesh::DrawCube({ -50.0 });

	//Graphics::Camera::camera->End();

	//Graphics::Camera::camera->GetFrameBuffer().BindColorAsTexture();
	//Shader::screenShader->SetUniform1i("_MainTex", 0);
	//Shader::screenShader->SetUniformVector2f("_ScreenAspect", Vector2(1, 1));*/
	//Mesh::DrawQuad({ -50.0, -50.0 });
	//FrameBuffer::UnbindTexture();
}

void GraphicsSystem::LateUpdate()
{
	SwapBuffers(windowDC); // using double buffering
}

void GraphicsSystem::Exit()
{
	//delete Input::KeyboardInput::inst;
	/*delete Graphics::Camera::camera;
	delete Shader::defaultShader;
	delete Shader::screenShader;*/
	//Mesh::Exit();

	std::cerr << "Shutting down OpenGL..." << std::endl;
	CleanRenderingEnvironment();
	std::cerr << "Shutting down OpenGL successful" << std::endl;
}

bool GraphicsSystem::InitializeRenderingEnvironment()
{
	//create rendering window
	windowDC = GetDC(_hwnd);

	DEVMODE devMode = { 0 };
	devMode.dmSize = sizeof(DEVMODE);
	BOOL b = EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);
	if (b == 0)
		return false;

	//drawing surface format
	PIXELFORMATDESCRIPTOR pfdesc;
	memset(&pfdesc, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfdesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfdesc.nVersion = 1;
	pfdesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
	pfdesc.iPixelType = PFD_TYPE_RGBA;
	pfdesc.cColorBits = static_cast<BYTE>(devMode.dmBitsPerPel); //32; //24 bit color for front and back buffer
	pfdesc.cDepthBits = 24; //24 bit depth buffer - not used in this demo
	pfdesc.cStencilBits = 8; //8 bit stencil buffer - not used in this demo

	int pf = ChoosePixelFormat(windowDC, &pfdesc);
	//checks if the graphics card can support the pixel format requested
	if (!pf)
	{
		ReleaseDC(_hwnd, windowDC);
		return false;
	}

	BOOL ok = SetPixelFormat(windowDC, pf, &pfdesc);
	if (!ok)
	{
		ReleaseDC(_hwnd, windowDC);
		return false;
	}

	//set the OpenGL context
	wglDC = wglCreateContext(windowDC);
	if (!wglDC)
	{
		ReleaseDC(_hwnd, windowDC);
		return false;
	}

	ok = wglMakeCurrent(windowDC, wglDC);
	if (!ok)
	{
		wglDeleteContext(wglDC);
		ReleaseDC(_hwnd, windowDC);
		return false;
	}

	return true;
}

void GraphicsSystem::CleanRenderingEnvironment()
{
	if (wglDC)
	{
		if (!wglMakeCurrent(nullptr, nullptr))
		{
			//log
		}
	}

	if (!wglDeleteContext(wglDC))
	{
		//log
	}
	wglDC = nullptr;

	if (windowDC && !ReleaseDC(_hwnd, windowDC))
	{
		windowDC = nullptr;
	}
}