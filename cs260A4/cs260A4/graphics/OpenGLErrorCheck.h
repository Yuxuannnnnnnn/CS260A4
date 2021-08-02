#pragma once
#include <GL/glew.h>
#include <iostream>

namespace Graphics
{
#define ASSERT(e) ((void)(0))
#define ASSERT_ERROR(e, ...)             \
  if (e)                                 \
  {                                      \
  }                                      \
  else                                   \
  {                                      \
    std::cerr << (__VA_ARGS__); \
  }
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

  void GLClearError();

  bool GLLogCall(const char* function, const char* file, int line);
}
