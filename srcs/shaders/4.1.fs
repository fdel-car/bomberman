#version 410 core
out vec4 fragColor;

in vec3 _normal;
in vec3 _fragPos;

uniform vec3 lightDir;
uniform vec3 cameraPos;
uniform vec3 lightColor;

struct s_material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

uniform s_material material;

void main() {
    // Ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * material.ambientColor * lightColor;

    // Difuse
    float diffCoeff = max(dot(_normal, -lightDir), 0.0f);
    vec3 diffuse = diffCoeff * material.diffuseColor * lightColor;

    // Specular
    vec3 viewDir = normalize(cameraPos - _fragPos);
    vec3 reflectDir = reflect(lightDir, _normal);
    float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specularCoeff * material.specularColor * lightColor;

    fragColor = vec4(ambient + diffuse + specular, 1.0f);
}