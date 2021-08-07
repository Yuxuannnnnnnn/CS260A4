//#include "Shader.h"
//#include "OpenGLErrorCheck.h"
//
//#include "../Dep/glew/GL/glew.h"
//
//#include <fstream>
//#include <iostream>
//#include <string>
//
//Shader* Shader::defaultShader = nullptr;
//Shader* Shader::screenShader = nullptr;
//
//Shader::Shader(std::string shaderFilepath)
//	: Shader(shaderFilepath + ".vs", shaderFilepath + ".fs") {}
//
//Shader::Shader(std::string vsfilepath, std::string fsfilepath)
//	: filepath{ vsfilepath, fsfilepath }, rendererID{ 0 }
//{
//	rendererID = CreateShader(LoadShader(vsfilepath), LoadShader(fsfilepath));
//}
//
//Shader::~Shader()
//{
//	std::cerr << "Deleting Shader " << filepath.vertexSource << " and " << filepath.fragmentSource << "..." << std::endl;
//	glDeleteProgram(rendererID);
//	std::cerr << "Shaders deleted" << std::endl;
//}
//
//void Shader::Bind() const
//{
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	GLCall(glUseProgram(rendererID));
//}
//
//void Shader::Unbind() const
//{
//	GLCall(glUseProgram(0));
//}
//
//std::string Shader::LoadShader(std::string shaderFilepath)
//{
//	std::ifstream file;
//	std::cerr << "Loading shader \"" << shaderFilepath.c_str() << "\"..." << std::endl;
//	file.open(shaderFilepath.c_str());
//
//	std::string output;
//	std::string line;
//
//	if (file.is_open())
//	{
//		while (file.good())
//		{
//			getline(file, line);
//			output.append(line + "\n");
//		}
//		std::cerr << "Shader \"" << shaderFilepath.c_str() << "\" was loaded successfully." << std::endl;
//	}
//	else
//	{
//		std::cerr << "Unable to load shader \"" << shaderFilepath.c_str() << "\"!" << std::endl;
//	}
//
//	return output;
//}
//
//unsigned Shader::CompileShader(unsigned type, std::string source)
//{
//	unsigned id = glCreateShader(type);
//	const char* src = source.c_str();
//	GLCall(glShaderSource(id, 1, &src, nullptr));
//	GLCall(glCompileShader(id));
//
//	//error handling
//	int result;
//	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
//	if (!result)
//	{
//		int length;
//		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
//		char* message = new char[length];
//		GLCall(glGetShaderInfoLog(id, length, &length, message));
//		std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
//		std::cerr << message << std::endl;
//		GLCall(glDeleteShader(id));
//		return 0;
//	}
//
//	return id;
//}
//
//unsigned Shader::CreateShader(std::string vertexShader, std::string fragmentShader)
//{
//	unsigned program = glCreateProgram();
//	unsigned vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//	unsigned fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//
//	if (!vs)
//		vs = CompileShader(GL_VERTEX_SHADER, LoadShader("Shaders/ErrorShader.vs"));
//	if (!fs)
//		fs = CompileShader(GL_FRAGMENT_SHADER, LoadShader("Shaders/ErrorShader.fs"));
//
//	GLCall(glAttachShader(program, vs));
//	GLCall(glAttachShader(program, fs));
//
//	GLCall(glBindAttribLocation(program, 0, "_MainTex"));
//	GLCall(glBindAttribLocation(program, 2, "_DispTex"));
//	GLCall(glBindAttribLocation(program, 4, "_NextTex"));
//
//	GLCall(glLinkProgram(program));
//	GLCall(glValidateProgram(program));
//
//	GLCall(glDeleteShader(vs));
//	GLCall(glDeleteShader(fs));
//
//	return program;
//}
//
//unsigned Shader::GetShaderID()
//{
//	return rendererID;
//}
//
//void Shader::SetUniform1i(std::string name, int i0)
//{
//	Bind();
//	glUniform1i(GetUniformLocation(name), i0);
//}
//
//void Shader::SetUniform1f(std::string name, float f0)
//{
//	Bind();
//	glUniform1f(GetUniformLocation(name), f0);
//}
//
//void Shader::SetUniformMatrix4f(std::string name, Matrix4x4 matrix)
//{
//	Bind();
//	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix.m);
//}
//
//void Shader::SetUniformColor4f(std::string name, Color c)
//{
//	Bind();
//	GLCall(glUniform4f(GetUniformLocation(name), c.r, c.g, c.b, c.a));
//}
//
//void Shader::SetUniformVector2f(std::string name, Vector2 v)
//{
//	Bind();
//	GLCall(glUniform2f(GetUniformLocation(name), v.x, v.y));
//}
//
//void Shader::SetUniformVector3f(std::string name, Vector3 v)
//{
//	Bind();
//	GLCall(glUniform3f(GetUniformLocation(name), v.x, v.y, v.z));
//}
//
//void Shader::SetUniformVector4f(std::string name, Vector4 v)
//{
//	Bind();
//	GLCall(glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w));
//}
//
//void Shader::SetUniformVector4f(std::string name, float v0, float v1, float v2, float v3)
//{
//	Bind();
//	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
//}
//
//int Shader::GetUniformLocation(std::string name)
//{
//	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
//		return m_uniformLocationCache[name];
//
//	GLCall(int location = glGetUniformLocation(rendererID, name.c_str()));
//	if (location == -1)
//		std::cerr << "Warning: uniform '" << name << "' doesn't exist in shader " << filepath.fragmentSource << std::endl;
//	m_uniformLocationCache[name] = location;
//	return location;
//}