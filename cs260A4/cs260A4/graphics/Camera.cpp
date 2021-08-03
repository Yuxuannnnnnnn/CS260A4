#include "Camera.h"
#include "OpenGLErrorCheck.h"

namespace Graphics
{
  Camera* Camera::camera = nullptr;

  Camera::Camera() :
    frameBuffer{},
    camWidth{ 1920 },
    aspect{ 16.f / 9.f },
    aspectFixed{ 0 },
    size{ 5 },
    zNear{ 0.3f },
    zFar{ 1000.f },
    backgroundColor{ Color(49 / 255.0f, 77 / 255.0f, 121 / 255.0f) },
    perspective{ false } {}

  void Camera::Begin()
  {
    frameBuffer.Bind();

    glEnable(GL_DEPTH_TEST);
    glClearColor(
      backgroundColor.r,
      backgroundColor.g,
      backgroundColor.b,
      1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
  void Camera::End()
  {
    frameBuffer.Unbind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
  }
  const FrameBuffer& Camera::GetFrameBuffer() const
  {
    return frameBuffer;
  }

  Matrix4x4 Camera::GetOrthographicMatrix() const
  {
    return Matrix4x4::Ortho(
      -size * aspect,
      size * aspect,
      -size, size, zNear, zFar);
  }
  Matrix4x4 Camera::GetPerspectiveMatrix() const
  {
    return Matrix4x4::Perspective(
      60,
      (aspectFixed > .01f) ? aspectFixed : aspect,
      zNear, zFar);
  }

  Matrix4x4 Camera::GetProjectionMatrix()
  {
    if (zFar < zNear)
      zFar = zNear;
    if (perspective)
      return GetPerspectiveMatrix();
    else
      return GetOrthographicMatrix();
  }

  Matrix4x4 Camera::GetViewMatrix(Transform transform) const
  {
    Matrix4x4 ret =  Matrix4x4::Rotate(transform.rotation);
    ret.m30 = -Vector3::Dot(Vector3(ret.m00, ret.m10, -ret.m20), transform.position);
    ret.m31 = -Vector3::Dot(Vector3(ret.m01, ret.m11, -ret.m21), transform.position);
    ret.m32 = -Vector3::Dot(Vector3(ret.m02, ret.m12, -ret.m22), transform.position);
    return ret;
  }
}
