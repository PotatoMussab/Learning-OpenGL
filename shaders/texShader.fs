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
struct Light{
    vec3 pos;
    vec3 color;
    vec3 intensity;
    };
uniform vec3 cameraPos;
uniform Light light;
uniform Material material;

void main()
{
    vec3 diffuseMap = texture(material.diffuseTexSampler, vTexCoords).rgb;
    //ambient component
    vec3 ambient = light.intensity[0] * diffuseMap;
    //diffuse component
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(light.pos - vPos);
    float diffuseCoeff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.intensity[1] * diffuseCoeff * diffuseMap;
    //specular component
    vec3 reflectedLightDir = reflect(-lightDir , norm);
    vec3 viewDir = normalize(cameraPos - vPos);
    float specularCoeff = pow(max(dot(reflectedLightDir, viewDir), 0.0f), material.shininess);
    vec3 specularMap = texture(material.specularTexSampler, vTexCoords).rgb;
    vec3 specular = light.intensity[2] * specularCoeff * specularMap;

    vec3 result = (ambient + diffuse + specular) * light.color;
    fColor = vec4(result, 1.0f);
}