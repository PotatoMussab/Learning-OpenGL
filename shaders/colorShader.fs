#version 330 core
in vec3 vNormal;
in vec3 vPos;

out vec4 fColor;
struct Material{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    };

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform Material material;

void main()
{
    vec3 ambient = material.ambientColor;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vPos);
    vec3 diffuse = material.diffuseColor * max(dot(norm, lightDir), 0.0f);

    vec3 reflectedLightDir = reflect(-lightDir , norm);
    vec3 viewDir = normalize(cameraPos - vPos);
    vec3 specular = material.specularColor * pow(max(dot(reflectedLightDir, viewDir), 0.0f), material.shininess);

    vec3 result = (ambient + diffuse + specular) * lightColor;
    fColor = vec4(result, 1.0f);
}