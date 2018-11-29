#version 410 core
out vec4 fragColor;

in vec3 _normal;
in vec3 _fragPos;
in vec2 _texCoords;
in vec4 _fragPosLightSpace;

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    bool hasDiffuseTexture;
    float shininess;
};

uniform Material material;

float shadowCalculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 10.0;
    // if (projCoords.z > 1.0)
        // shadow = 0.0;
    return shadow;
}

void main() {
    // Ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * material.ambientColor * lightColor;

    // Difuse
    vec3 diffuse;
    float diffCoeff = max(dot(_normal, -lightDir), 0.0f);
    if (material.hasDiffuseTexture)
        diffuse = diffCoeff * texture(diffuseTexture, _texCoords).xyz * lightColor;
    else
        diffuse = diffCoeff * material.diffuseColor * lightColor;

    // Specular
    vec3 specular = vec3(0.0f);
    if (diffCoeff > 0.0f) {
        vec3 viewDir = normalize(viewPos - _fragPos);
        // Blinn-Phong model
        vec3 halfDir = normalize(-lightDir + viewDir);
        float specularCoeff = pow(max(dot(_normal, halfDir), 0.0), material.shininess);
        // // Phong model
        // vec3 reflectDir = reflect(lightDir, _normal);
        // float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        specular = specularCoeff * material.specularColor * lightColor;
    }

    // Shadow
    float shadow = shadowCalculation(_fragPosLightSpace);
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular));

    fragColor = vec4(result , 1.0f);
}