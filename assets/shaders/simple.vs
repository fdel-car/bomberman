#version 410 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

out vec3 _normal;

void main()
{
    gl_Position = vec4(pos, 1.0f);
    _normal = normal;
}