#include "GraphicsEngine.h"
#include <GL/glew.h>
#include "PCH.h"
#include "OpenGLErrorCheck.h"
#include "Graphics3d.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Input/KeyboardInput.h"

HDC windowDC{}; // global Window-device context
HGLRC wglDC{}; // OpenGL-device context

namespace Graphics
{
  bool InitializeRenderingEnvironment();
  void CleanRenderingEnvironment();

  void Init()
  {
    std::cerr << "Initializing Rendering Environment..." << std::endl;
    InitializeRenderingEnvironment();

    glClearColor(0.5f, 0, 0, 1); //RGBA

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
      std::cerr << "glewInit() failed!" << std::endl;
    }
    else
    {
      std::cerr << "Initializing OpenGL successful" << std::endl;
      std::cerr << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    }

    Mesh::Init();
    Shader::defaultShader = new Shader("Assets/Shaders/Default.vs", "Assets/Shaders/PBR.fs");
    Shader::screenShader = new Shader("Assets/Shaders/DefaultScreen");
    Camera::camera = new Camera;
    Input::KeyboardInput::inst = new Input::KeyboardInput;
  }
  Transform objTransform{};
  Transform objTransform2{Vector3(0, -1, 0)};
  Transform camTransform{ Vector3(0, 0, -10) };
  Transform lightTransform{ Vector3(-10, 0, 0) };
  void Update()
  {
    glEnable(GL_DEPTH_TEST);
    //update opengl
    glClearColor(0.5f, 0, 0, 1); //RGBA
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RECT rect;
    GetClientRect(hwnd, &rect);
    glViewport(static_cast<GLint>(0), static_cast<GLint>(0), rect.right - rect.left, rect.bottom - rect.top);

    // input
    Input::KeyboardInput::inst->Update();
    if (Input::KeyboardInput::inst->IsKeyTriggered(VK_1))
      Camera::camera->perspective = false;
    if (Input::KeyboardInput::inst->IsKeyTriggered(VK_2))
      Camera::camera->perspective = true;

    if (Input::KeyboardInput::inst->IsKeyPressed(VK_A))
      camTransform.position += camTransform.Right() / 200;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_D))
      camTransform.position += camTransform.Left() / 200;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_S))
      camTransform.position += camTransform.Back() / 200;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_W))
      camTransform.position += camTransform.Forward() / 200;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_Q))
      camTransform.position += camTransform.Down() / 200;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_E))
      camTransform.position += camTransform.Up() / 200;

    if (Input::KeyboardInput::inst->IsKeyPressed(VK_J))
      objTransform.position.x += -.001953125f;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_L))
      objTransform.position.x += .001953125f;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_K))
      objTransform.position.y += -.001953125f;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_I))
      objTransform.position.y += .001953125f;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_U))
      objTransform.position.z += -.001953125f;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_O))
      objTransform.position.z += .001953125f;

    static Vector3 euler1 = Vector3();
    static Vector3 euler2 = Vector3();

    if (Input::KeyboardInput::inst->IsKeyPressed(VK_7))
      euler1.y += -0.125f;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_8))
      euler1.y += 0.125f;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_9))
      euler2.y += -0.125f;
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_0))
      euler2.y += 0.125f;

    camTransform.rotation = Quaternion::EulerAnglesToQuaternion(euler1);
    objTransform.rotation = Quaternion::EulerAnglesToQuaternion(euler2);

    if (Input::KeyboardInput::inst->IsKeyPressed(VK_3))
      camTransform.rotation = Quaternion::Identity();
    if (Input::KeyboardInput::inst->IsKeyPressed(VK_4))
      objTransform.rotation = Quaternion::Identity();

    //euler1 = Quaternion::QuaternionToEulerAngles(camTransform.rotation);
    //euler2 = Quaternion::QuaternionToEulerAngles(objTransform.rotation);

    // test mesh draw
    Camera::camera->Begin();

    Matrix4x4 proj = Camera::camera->GetProjectionMatrix();
    Matrix4x4 view = Camera::camera->GetViewMatrix(camTransform);
    Matrix4x4 model = objTransform.LocalToWorldMatrix();

    Shader::defaultShader->SetUniformMatrix4f("_M", model);
    Shader::defaultShader->SetUniformMatrix4f("_V", view);
    Shader::defaultShader->SetUniformMatrix4f("_P", proj);
    Shader::defaultShader->SetUniformVector3f("lightPos", camTransform.position);
    Shader::defaultShader->SetUniformVector3f("lightPos", camTransform.position);
    Shader::defaultShader->SetUniformVector3f("lightPos", camTransform.position);
    Shader::defaultShader->SetUniformVector3f("camPos", camTransform.position);

    Mesh::DrawCube();

    //model = objTransform2.LocalToWorldMatrix();
    //Shader::defaultShader->SetUniformMatrix4f("_M", model);
    //Mesh::DrawCube();

    Camera::camera->End();

    Camera::camera->GetFrameBuffer().BindColorAsTexture();
    Shader::screenShader->SetUniform1i("_MainTex", 0);
    Shader::screenShader->SetUniformVector2f("_ScreenAspect", Vector2(1, 1));
    Mesh::DrawQuad();
    FrameBuffer::UnbindTexture();
  }
  void LateUpdate()
  {
    SwapBuffers(windowDC); // using double buffering
  }
  void Exit()
  {
    delete Input::KeyboardInput::inst;
    delete Camera::camera;
    delete Shader::defaultShader;
    delete Shader::screenShader;
    Mesh::Exit();

    std::cerr << "Shutting down OpenGL..." << std::endl;
    CleanRenderingEnvironment();
    std::cerr << "Shutting down OpenGL successful" << std::endl;
  }

  bool InitializeRenderingEnvironment()
  {
    //create rendering window
    windowDC = GetDC(hwnd);

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
      ReleaseDC(hwnd, windowDC);
      return false;
    }


    BOOL ok = SetPixelFormat(windowDC, pf, &pfdesc);
    if (!ok)
    {
      ReleaseDC(hwnd, windowDC);
      return false;
    }


    //set the OpenGL context
    wglDC = wglCreateContext(windowDC);
    if (!wglDC)
    {
      ReleaseDC(hwnd, windowDC);
      return false;
    }


    ok = wglMakeCurrent(windowDC, wglDC);
    if (!ok)
    {
      wglDeleteContext(wglDC);
      ReleaseDC(hwnd, windowDC);
      return false;
    }

    return true;
  }

  void CleanRenderingEnvironment()
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

    if (windowDC && !ReleaseDC(hwnd, windowDC))
    {
      windowDC = nullptr;
    }
  }
}
