#version 410 core
layout (location = 0) in vec3 position;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texCoords = position;
    vec4 position = projection * view * vec4(position, 1.0);
    gl_Position = position.xyww;
}  