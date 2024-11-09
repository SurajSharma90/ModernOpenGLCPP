#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

out vec3 vertexColor;

void main()
{
    //vertexColor = clamp(vPos, 0.0f, 1.0f);
    vertexColor = vColor;
    gl_Position = projectionMatrix * modelMatrix * vec4(vPos, 1.0);
};