#version 410 core
layout (location = 0) in vec3 position;
layout (location = 3) in ivec4 jointIds;
layout (location = 4) in vec4 weights;

uniform mat4 lightSpaceMatrix;
uniform mat4 M;
uniform mat4 boneTransforms[32];
uniform bool rigged;

void main()
{
    if (rigged) {
        mat4 jointTransform = boneTransforms[jointIds[0]] * weights[0];
            jointTransform += boneTransforms[jointIds[1]] * weights[1];
            jointTransform += boneTransforms[jointIds[2]] * weights[2];
            jointTransform += boneTransforms[jointIds[3]] * weights[3];
        gl_Position = lightSpaceMatrix * M * jointTransform * vec4(position, 1.0);
    } else
        gl_Position = lightSpaceMatrix * M * vec4(position, 1.0);
}  