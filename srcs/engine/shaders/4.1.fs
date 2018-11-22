#version 410 core
out vec4 fragColor;

in vec3 _normal;
in vec3 _fragPos;
in vec2 _texCoords;
in vec4 _fragPosLightSpace;

uniform vec3 lightDir;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;

uniform sampler2D textureID;
uniform sampler2D shadowMap;

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    bool isTextured;
    float shininess;
};

uniform Material material;

float shadowCalculation(vec4 fragPosLightSpace, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth  > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 10.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main() {
    // Ambient
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * material.ambientColor;// * lightColor;

    // Difuse
    // vec3 diffuse;
    // float diffCoeff = max(dot(_normal, -lightDir), 0.0f);
    // if (material.isTextured)
    //     diffuse = diffCoeff * texture(textureID, _texCoords).xyz * lightColor;
    // else
    //     diffuse = diffCoeff * material.diffuseColor * lightColor;

    vec3 diffuse;
    // vec3 lightDire = normalize(lightPos - _fragPos);
    float diffCoeff = max(dot(_normal, -lightDir), 0.0f);
    if (material.isTextured)
        diffuse = diffCoeff * texture(textureID, _texCoords).xyz;// * lightColor;
    else
        diffuse = diffCoeff * material.diffuseColor;// * lightColor;



    // // Specular
    // vec3 viewDir = normalize(cameraPos - _fragPos);
    // vec3 reflectDir = reflect(lightDir, _normal);
    // float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = specularCoeff * material.specularColor * lightColor;

    // fragColor = vec4(ambient + diffuse + specular, 1.0f);
    // fragColor = vec4(ambient + diffuse, 1.0f);

    // Shadow
    float shadow = shadowCalculation(_fragPosLightSpace, lightDir);
    vec3 result = (ambient + (1.0 - shadow) * (diffuse));

    fragColor = vec4(result , 1.0f);
   
}