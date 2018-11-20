#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 _normal;
out vec3 _fragPos;

uniform mat4 PV;
uniform mat4 M;

void main()
{
    gl_Position = PV * M * vec4(position, 1.0f);
    _normal = normalize(normal);
    _fragPos = vec3(M * vec4(position, 1.0f));
}