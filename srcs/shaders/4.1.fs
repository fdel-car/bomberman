#version 410 core
out vec4 fragColor;

in vec3 _normal;
in vec3 _color;
in vec3 _fragPos;

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main() {
    // Ambient
    float ambientStrength = 0.15f;
    vec3 ambient = ambientStrength * lightColor;

    // Difuse
    float diffCoeff = max(dot(_normal, -lightDir), 0.0f);
    vec3 diffuse = diffCoeff * lightColor;

    // Specular
    float specularStrength = 2.0f;
    vec3 viewDir = normalize(viewPos - _fragPos);
    vec3 reflectDir = reflect(lightDir, _normal);
    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * specCoeff * lightColor;

    vec3 result = (ambient + diffuse + specular) * _color;
    fragColor = vec4(result , 1.0f);
}