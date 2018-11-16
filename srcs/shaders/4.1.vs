#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

out vec3 _normal;
out vec3 _color;
out vec3 _fragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    _normal = normalize(normal);
    _color = color;
    _fragPos = vec3(model * vec4(position, 1.0f));
}