#version 330 core
in vec3 vNormal;
in vec3 vPos;

out vec4 fColor;

uniform vec3 color;
uniform vec3 lightPos;

void main()
{
    float ambient = 0.125f;
    float diffStrength = 0.875f;

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vPos);
    float diff = diffStrength * max(dot(norm, lightDir), 0.0f);

    vec3 result = (ambient + diff) * color;
    fColor = vec4(result, 1.0f);
}