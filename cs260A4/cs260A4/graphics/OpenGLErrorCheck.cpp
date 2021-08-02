#include "OpenGLErrorCheck.h"
#include <iostream>

namespace Graphics
{
  void GLClearError()
  {
    while (glGetError() != GL_NO_ERROR);
  }

  bool GLLogCall(const char* function, const char* file, int line)
  {
    while (GLenum error = glGetError())
    {
      std::cerr << ("OpenGL error code {}: {} {}:{}", error, function, file, line);
      return false;
    }
    return true;
  }
}
