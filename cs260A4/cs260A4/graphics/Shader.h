/**********************************************************************************
* \file   Shader.h
* \brief  The file contains interface of Shader class.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "PCH.h"
#include "Math/Math.h"

namespace Graphics
{
  struct ShaderProgramSource
  {
    std::string vertexSource;
    std::string fragmentSource;
  };

  class Shader
  {
    ShaderProgramSource filepath;
    unsigned rendererID;
    std::unordered_map<std::string, int> m_uniformLocationCache;
  public:
    static Shader* defaultShader;
    static Shader* screenShader;

    Shader(std::string filepath);
    Shader(std::string vsfilepath, std::string fsfilepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    unsigned GetShaderID();
    std::string GetName();
    // Set uniforms
    void SetUniform1i(std::string name, int i0);
    void SetUniform1f(std::string name, float f0);
    void SetUniformMatrix4f(std::string name, Matrix4x4 matrix);
    void SetUniformColor4f(std::string name, Color c);
    void SetUniformVector2f(std::string name, Vector2 v);
    void SetUniformVector3f(std::string name, Vector3 v);
    void SetUniformVector4f(std::string name, Vector4 v);
    void SetUniformVector4f(std::string name, float v0, float v1, float v2, float v3);
  private:
    std::string LoadShader(std::string filepath);
    unsigned CompileShader(unsigned type, std::string source);
    unsigned CreateShader(std::string vertexShader, std::string fragmentShader);
    int GetUniformLocation(std::string name);
  };
}
