#version 330 core

in vec3 vertexPosition;
in vec4 vertexColor;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
out vec4 color;

uniform sampler2D _MainTex;
uniform vec4 _Color = vec4(0, 1, 1, 1);

uniform vec3 lightPos = vec3(0, 0, 0);
uniform vec3 lightColor = vec3(1, 1, 1);

uniform vec3 viewPos = vec3(0, 0, 0);

void main()
{
  //color = texture(_MainTex, vertexTexCoord) * vertexColor * _Color;

  // ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;
 
  // diffuse 
  vec3 norm = normalize(vertexNormal);
  vec3 lightDir = normalize(lightPos - vertexPosition);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  
  // specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - vertexPosition);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;  
      
  vec3 result = (ambient + diffuse + specular) * vec3(0.5,0,0);
  color = vec4(result, 1.0);

  //color = _Color;
}
