#version 330 core
in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;

out vec4 color;

uniform sampler2D _MainTex;
uniform vec4 _Color = vec4(1, 1, 1, 1);

void main()
{
  color = texture(_MainTex, vertexTexCoord) * _Color;
} 