#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 _normal;
out vec3 _fragPos;
out vec2 _texCoords;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    gl_Position = P * V * M * vec4(position, 1.0f);
    // Look up transpose(inverse(M)), this works now but it won't always do
    _normal = normalize(M * vec4(normal, 0.0f)).xyz;
    // _normal = normalize(normal);
    _fragPos = vec3(M * vec4(position, 1.0f));
    _texCoords = texCoords;
}