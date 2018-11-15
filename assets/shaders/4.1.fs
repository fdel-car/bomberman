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
    // vec3 lightDir = normalize(lightDir - _fragPos);
    vec3 normLightDir = normalize(-lightDir);
    float diff = max(dot(_normal, normLightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 2.0f;
    vec3 viewDir = normalize(viewPos - _fragPos);
    vec3 reflectDir = reflect(-normLightDir, _normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * _color;
    fragColor = vec4(result , 1);
    // fragColor = vec4(_color, 1.0f);
}