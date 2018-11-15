#version 410 core
out vec4 fragColor;

in vec3 _normal;
in vec3 _color;

void main() {
    fragColor = vec4(_color.x, _color.y, _color.z, 1.0f);
}