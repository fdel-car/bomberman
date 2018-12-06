#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in ivec4 jointIds;
layout (location = 4) in vec4 weights;

out vec3 _normal;
out vec3 _fragPos;
out vec2 _texCoords;
out vec4 _fragPosLightSpace;

out vec4 _weights;
out vec4 _ids;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = P * V * M * vec4(position, 1.0f);
    // Look up transpose(inverse(M)), this works now but it won't always do
    _normal = normalize(M * vec4(normal, 0.0f)).xyz;
    _fragPos = vec3(M * vec4(position, 1.0f));
    _texCoords = texCoords;
    _fragPosLightSpace = lightSpaceMatrix * vec4(_fragPos, 1.0f);

    _ids = jointIds;
    _weights = weights;
}