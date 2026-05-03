#version 330 core
in vec3 vNormal;
in vec3 vPos;

out vec4 fColor;

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
    float ambient = 0.1f;
    float diffStrength = 0.7f;
    float specularStrength = 0.5f;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vPos);
    float diffuse = diffStrength * max(dot(norm, lightDir), 0.0f);

    vec3 reflectedLightDir = reflect(-lightDir , norm);
    vec3 viewDir = normalize(cameraPos - vPos);
    float specular = specularStrength * pow(max(dot(reflectedLightDir, viewDir), 0.0f), 32);

    vec3 result = (ambient + diffuse + specular) * color;
    fColor = vec4(result, 1.0f);
}