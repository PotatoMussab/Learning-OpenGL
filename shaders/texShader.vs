#version 330 core
layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inNormal;
layout (location=2) in vec2 inTexCoords;

out vec3 vNormal;
out vec3 vPos;
out vec2 vTexCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
vec4 modelPos = model * vec4(inPos, 1.0f);
gl_Position = proj * view * modelPos;
vPos = modelPos.xyz;
vNormal = mat3(model) * inNormal;
vTexCoords = vec2(inTexCoords);
}