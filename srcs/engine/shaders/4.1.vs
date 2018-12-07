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

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 lightSpaceMatrix;
uniform mat4 boneTransforms[32];
uniform bool rigged;

void main()
{
    if (rigged) {
        mat4 jointTransform = boneTransforms[jointIds[0]] * weights[0];
            jointTransform += boneTransforms[jointIds[1]] * weights[1];
            jointTransform += boneTransforms[jointIds[2]] * weights[2];
            jointTransform += boneTransforms[jointIds[3]] * weights[3];
        gl_Position = P * V * M * jointTransform * vec4(position, 1.0f);
        _normal = normalize(M * jointTransform * vec4(normal, 0.0f)).xyz;
        _fragPos = vec3(M * jointTransform * vec4(position, 1.0f));
    } else {
        gl_Position = P * V * M * vec4(position, 1.0f);
        _normal = normalize(M * vec4(normal, 0.0f)).xyz;
        _fragPos = vec3(M * vec4(position, 1.0f));
    }
    // Look up transpose(inverse(M)), this works now but it won't always do
    _texCoords = texCoords;
    _fragPosLightSpace = lightSpaceMatrix * vec4(_fragPos, 1.0f);
}