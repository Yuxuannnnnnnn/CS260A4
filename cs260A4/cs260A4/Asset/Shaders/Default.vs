#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;

out vec3 WorldPos;
out vec4 FragColor;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 _M; //ModelMatrix
uniform mat4 _V; //ViewMatrix
uniform mat4 _P; //ProjectionMatrix

void main()
{
  vec4 v = vec4(position, 1.0);
  vec4 v1 = _M * v;
  vec4 v2 = _V * v1;
  vec4 v3 = _P * v2;
  gl_Position = v3;
  
  WorldPos = v1.xyz;
  FragColor = color;
  TexCoords = texCoord;
  Normal = mat3(transpose(_M)) * normal;
}
