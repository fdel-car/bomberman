#version 410 core
out vec4 fragColor;

in vec3 _normal;
in vec3 _color;
in vec3 _fragPos;

// uniform vec3 lightDir;
// uniform vec3 viewPos;
// uniform vec3 lightColor;

void main() {
    //ambient
    // float ambientStrength = 0.1f;
    // vec3 ambient = ambientStrength * lightColor;

    // //difuse
    // vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(-lightDir);
    // float diff = max(dot(norm, lightDir), 0.0f);
    // vec3 diffuse = diff * lightColor;

    // // specular
    // float specularStrength = 2.0f;
    // vec3 viewDir = normalize(viewPos - _fragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = specularStrength * spec * lightColor;

    // vec3 result = (ambient + diffuse + specular) * _color;
    // fragColor = vec4(result , 1);

    fragColor = vec4(_color, 1.0f);
}