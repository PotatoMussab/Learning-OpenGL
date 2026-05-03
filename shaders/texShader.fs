#version 330 core
in vec3 vNormal;
in vec3 vPos;
in vec2 vTexCoords;

out vec4 fColor;

struct Material{
    sampler2D diffuseTexSampler;
    sampler2D specularTexSampler;
    float shininess;
    };

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 lightIntensity;
uniform Material material;

void main()
{
    vec3 diffuseMap = texture(material.diffuseTexSampler, vTexCoords).rgb;
    //ambient component
    vec3 ambient = lightIntensity[0] * diffuseMap;
    //diffuse component
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vPos);
    float diffuseCoeff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = lightIntensity[1] * diffuseCoeff * diffuseMap;
    //specular component
    vec3 reflectedLightDir = reflect(-lightDir , norm);
    vec3 viewDir = normalize(cameraPos - vPos);
    float specularCoeff = pow(max(dot(reflectedLightDir, viewDir), 0.0f), material.shininess);
    vec3 specularMap = texture(material.specularTexSampler, vTexCoords).rgb;
    vec3 specular = lightIntensity[2] * specularCoeff * specularMap;

    vec3 result = (ambient + diffuse + specular) * lightColor;
    fColor = vec4(result, 1.0f);
}