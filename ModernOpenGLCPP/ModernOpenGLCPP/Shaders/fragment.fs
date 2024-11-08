#version 330 core

in vec3 vertexColor;

out vec4 FragColor;

uniform vec3 vColor;
uniform int useVertexColor;

void main()
{
  if(useVertexColor == 1)
    FragColor = vec4(vertexColor, 1.0f);
  else
    FragColor = vec4(vColor, 1.0f);
};