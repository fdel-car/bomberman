#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 M;

void main()
{
    gl_Position = lightSpaceMatrix * M * vec4(aPos, 1.0);
}  