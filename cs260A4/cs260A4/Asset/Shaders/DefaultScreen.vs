#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;

out vec3 vertexPosition;
out vec4 vertexColor;
out vec2 vertexTexCoord;

uniform vec2 _ScreenAspect;

void main()
{
  gl_Position = vec4(position.x * 2 * _ScreenAspect.x, -position.y * 2 * _ScreenAspect.y, 0.0, 1.0);
  
  vertexPosition = gl_Position.xyz;
  vertexColor = color;
  vertexTexCoord = texCoord;
}